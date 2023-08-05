// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SVrPawn.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SGameModeBase.h"
#include "NavigationSystem.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogSVrPawn, All, All);

ASVrPawn::ASVrPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(GetRootComponent());

    MotionControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>("MotionControllerLeft");
    MotionControllerLeft->MotionSource = FName(TEXT("Left"));
    MotionControllerLeft->SetupAttachment(GetRootComponent());

    MotionControllerRight = CreateDefaultSubobject<UMotionControllerComponent>("MotionControllerRight");
    MotionControllerRight->MotionSource = FName(TEXT("Right"));
    MotionControllerRight->SetupAttachment(GetRootComponent());

    StaticMeshComponentLeft = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponentLeft");
    StaticMeshComponentLeft->SetupAttachment(MotionControllerLeft);

    StaticMeshComponentRight = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponentRight");
    StaticMeshComponentRight->SetupAttachment(MotionControllerRight);

    PointerTrace = CreateDefaultSubobject<UNiagaraComponent>("PointerTrace");
    PointerTrace->SetupAttachment(GetRootComponent());

    WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>("WidgetInteraction");
    WidgetInteraction->SetupAttachment(PointerTrace);
    WidgetInteraction->InteractionDistance = 0.0f;
}

void ASVrPawn::BeginPlay()
{
    Super::BeginPlay();
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
            UEnhancedInputLocalPlayerSubsystem>(
            PC->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
    if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
    {
        UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
    }
    if (PointerTrace)
    {
        PointerTrace->SetVisibility(false);
        PointerTrace->SetVectorParameter(PropertyForDistanceAdjusting, FVector(PointerMaxDistance, 0.0f, 0.0f));

    }

    auto GameMode = GetWorld() ? Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
    if (GameMode)
    {
        UE_LOG(LogSVrPawn, Display, TEXT("Subscrybe on gamemode"));
        GameMode->OnGameStateChanged.AddUObject(this, &ASVrPawn::OnMatchStateChanged);
    }
}

void ASVrPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Turning
        PlayerEnhancedInputComponent->BindAction(ThumbstickTurning, ETriggerEvent::Started, this,
            &ASVrPawn::TurnPlayer);

        //Pointer
        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnRight, ETriggerEvent::Started, this,
            &ASVrPawn::ActivatePointer, ESPointerSourceSide::Right);
        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnLeft, ETriggerEvent::Started, this,
            &ASVrPawn::ActivatePointer, ESPointerSourceSide::Left);

        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnRight, ETriggerEvent::Triggered, this,
            &ASVrPawn::UpdatePointerTrace);
        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnLeft, ETriggerEvent::Triggered, this,
            &ASVrPawn::UpdatePointerTrace);

        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnRight, ETriggerEvent::Completed, this,
            &ASVrPawn::DisablePointer, ESPointerSourceSide::Right);
        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnLeft, ETriggerEvent::Completed, this,
            &ASVrPawn::DisablePointer, ESPointerSourceSide::Left);

        //WidgetInteraction
        PlayerEnhancedInputComponent->BindAction(TriggerClickOnRight, ETriggerEvent::Started, this,
            &ASVrPawn::InputData, ESPointerSourceSide::Right);
        PlayerEnhancedInputComponent->BindAction(TriggerClickOnLeft, ETriggerEvent::Started, this,
            &ASVrPawn::InputData, ESPointerSourceSide::Left);
    }
}

void ASVrPawn::TurnPlayer(const FInputActionValue& Value)
{
    if (Value[0] > 0.0f)
    {
        SnapTurn(true, TurningDegrees);
    }
    else if (Value[0] < 0.0f)
    {
        SnapTurn(false, TurningDegrees);
    }
}

void ASVrPawn::SnapTurn(bool IsRightTurn, float Degrees)
{
    auto Delta = IsRightTurn ? FRotator(0.0f, Degrees, 0.0f) : FRotator(0.0f, -1 * Degrees, 0.0f);
    AddActorWorldRotation(Delta);
}

void ASVrPawn::ActivatePointer(const ESPointerSourceSide MotionControllerSourceSide)
{
    if (!bIsPointerActive)
    {
        if (MotionControllerSourceSide == ESPointerSourceSide::Right)
        {
            AttachPointerToController(ESPointerSourceSide::Right, StaticMeshComponentRight);
        }
        else if (MotionControllerSourceSide == ESPointerSourceSide::Left)
        {
            AttachPointerToController(ESPointerSourceSide::Left, StaticMeshComponentLeft);
        }
    }
}

void ASVrPawn::AttachPointerToController(ESPointerSourceSide MotionControllerSourceSide, UMeshComponent* ControllerMesh)
{
    PointerTrace->SetVisibility(true);
    WidgetInteraction->InteractionDistance = PointerMaxDistance;
    FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
    PointerTrace->AttachToComponent(ControllerMesh, AttachmentTransformRules, PointerSocketName);
    PointerMotionSource = MotionControllerSourceSide;
    bIsPointerActive = true;
}

void ASVrPawn::InputData(const ESPointerSourceSide MotionControllerSourceSide)
{
    if (!WidgetInteraction || !bIsPointerActive || PointerMotionSource != MotionControllerSourceSide)
        return;
    WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
    WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

void ASVrPawn::OnMatchStateChanged(ESGameState GameState)
{
    if (GameState == ESGameState::InProgress)
    {
        bCanTeleport = true;
    }
    else
    {
        bCanTeleport = false;
    }
}

void ASVrPawn::DisablePointer(const ESPointerSourceSide MotionControllerSourceSide)
{
    UE_LOG(LogSVrPawn, Display, TEXT("HidePointer!"));
    if (PointerMotionSource == MotionControllerSourceSide)
    {
        TryToTeleportOnNewLocation();
        PointerTrace->SetVisibility(false);
        PointerMotionSource = ESPointerSourceSide::NONE;
        bIsPointerActive = false;
        WidgetInteraction->InteractionDistance = 0.0f;
    }
}

void ASVrPawn::UpdatePointerTrace()
{

    if (bCanTeleport && bIsPointerActive)
    {
        FVector SocketLocation;
        FRotator SocketRotation;
        PointerMotionSource == ESPointerSourceSide::Right
            ? StaticMeshComponentRight->GetSocketWorldLocationAndRotation(PointerSocketName, SocketLocation,
                SocketRotation)
            : StaticMeshComponentLeft->GetSocketWorldLocationAndRotation(PointerSocketName, SocketLocation,
                SocketRotation);

        auto ForwardVectorOfPointer = UKismetMathLibrary::GetForwardVector(SocketRotation);
        FVector EndLocation = SocketLocation + ForwardVectorOfPointer * PointerMaxDistance;

        FHitResult HitResult;
        auto IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, SocketLocation, EndLocation,
            ECollisionChannel::ECC_WorldStatic);

        auto Distance = FVector::Distance(SocketLocation, HitResult.Location);
        PointerTrace->SetVectorParameter(PropertyForDistanceAdjusting, FVector(Distance, 0.0f, 0.0f));

        if (IsHit)
        {
            //Находим ближайшую точку на навигационном мэше, если она есть...
            bValidLocationForTeleportWasFound = IsValidTeleportLocation(FoundedTeleportLocationOnNavMesh, HitResult.Location);
        }
        
        if (bValidLocationForTeleportWasFound)
        {
            PointerTrace->SetColorParameter(PropertyForColorAdjusting, ValidHitColor);
        }
        else
        {
            FoundedTeleportLocationOnNavMesh = FVector();
            PointerTrace->SetColorParameter(PropertyForColorAdjusting, InvalidHitColor);
        }

    }
}

bool ASVrPawn::IsValidTeleportLocation(FVector& ProjectedLocationOnNavMesh, FVector HitLocation) const
{
    FNavLocation NavLoc;
    if (auto NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem()))
    {
        bool bProjectedLocationValid = NavSystem->ProjectPointToNavigation(HitLocation, NavLoc, QueryingExtent);
        ProjectedLocationOnNavMesh = NavLoc.Location;
        return bProjectedLocationValid;
    }
    return false;
}

bool ASVrPawn::FindPointOnMeshFromNavMesh(FVector LocationOnNavMesh, FHitResult& TraceResult) const
{
    FHitResult Result;
    const auto WasHitMesh = GetWorld()->LineTraceSingleByChannel(Result,
        LocationOnNavMesh + FVector(
            0.0f, 0.0f, HeightOfVerticalLineFromNavMesh),
        LocationOnNavMesh - FVector(
            0.0f, 0.0f, 2 * HeightOfVerticalLineFromNavMesh),
        ECC_Visibility,
        FCollisionQueryParams(),
        FCollisionResponseParams());

    // DrawDebugLine(GetWorld(), LocationOnNavMesh + FVector(
    //         0.0f, 0.0f, HeightOfVerticalLineFromNavMesh),
    //     LocationOnNavMesh - FVector(
    //         0.0f, 0.0f, 2 * HeightOfVerticalLineFromNavMesh), FColor::Magenta, true, 100.0f, 0, 3.0f);

    if (WasHitMesh)
    {
        TraceResult = Result;
    }
    return WasHitMesh;
}


void ASVrPawn::TryToTeleportOnNewLocation()
{
    if (bCanTeleport && bValidLocationForTeleportWasFound && UHeadMountedDisplayFunctionLibrary::GetHMDWornState() == EHMDWornState::Worn)
    {
        FHitResult Result;
        bool FindPointOnMeshUnderNavMesh = FindPointOnMeshFromNavMesh(FoundedTeleportLocationOnNavMesh, Result);
        if (FindPointOnMeshUnderNavMesh)
        {
            SetActorLocation(Result.Location);
        }
    }
    bValidLocationForTeleportWasFound = false;
    FoundedTeleportLocationOnNavMesh = FVector();
}
