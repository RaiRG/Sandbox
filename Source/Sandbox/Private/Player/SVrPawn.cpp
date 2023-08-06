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
#include "SMotionController.h"
#include "SPickableUp.h"
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

    auto GameMode = GetWorld() ? Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
    if (GameMode)
    {
        GameMode->OnGameStateChanged.AddUObject(this, &ASVrPawn::OnMatchStateChanged);
    }

    LeftController = Cast<ASMotionController>(GetWorld()->SpawnActor(LeftMotionControllerClass));
    if (LeftController)
    {
        LeftController->SetOwner(this);
        LeftController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }

    RightController = Cast<ASMotionController>(GetWorld()->SpawnActor(RightMotionControllerClass));
    if (RightController)
    {
        RightController->SetOwner(this);
        RightController->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
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

        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnRight, ETriggerEvent::Completed, this,
            &ASVrPawn::DisablePointer, ESPointerSourceSide::Right);
        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnLeft, ETriggerEvent::Completed, this,
            &ASVrPawn::DisablePointer, ESPointerSourceSide::Left);

        //WidgetInteraction
        PlayerEnhancedInputComponent->BindAction(TriggerClickOnRight, ETriggerEvent::Started, this,
            &ASVrPawn::InputData, ESPointerSourceSide::Right);
        PlayerEnhancedInputComponent->BindAction(TriggerClickOnLeft, ETriggerEvent::Started, this,
            &ASVrPawn::InputData, ESPointerSourceSide::Left);

        //Interaction with world:
        PlayerEnhancedInputComponent->BindAction(GrabClickOnRight, ETriggerEvent::Started, this,
            &ASVrPawn::TryToInteractWithWorld, ESPointerSourceSide::Right);
        PlayerEnhancedInputComponent->BindAction(GrabClickOnLeft, ETriggerEvent::Started, this,
            &ASVrPawn::TryToInteractWithWorld, ESPointerSourceSide::Left);
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
    const auto Delta = IsRightTurn ? FRotator(0.0f, Degrees, 0.0f) : FRotator(0.0f, -1 * Degrees, 0.0f);
    AddActorWorldRotation(Delta);
}

void ASVrPawn::InputData(const ESPointerSourceSide MotionControllerSourceSide)
{
    MotionControllerSourceSide == ESPointerSourceSide::Right ? RightController->InputData() : LeftController->InputData();
}

void ASVrPawn::ActivatePointer(const ESPointerSourceSide MotionControllerSourceSide)
{
    if (bIsPointerActive)
        return;

    PointerMotionSource = MotionControllerSourceSide;
    bIsPointerActive = true;

    MotionControllerSourceSide == ESPointerSourceSide::Right ? RightController->ActivatePointer() : LeftController->ActivatePointer();

}


void ASVrPawn::TryToInteractWithWorld(const ESPointerSourceSide MotionControllerSourceSide)
{
    ESPointerSourceSide::Right == MotionControllerSourceSide ? RightController->InteractWithWorld() : LeftController->InteractWithWorld();
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
    if (PointerMotionSource == MotionControllerSourceSide)
    {
        TryToTeleportOnNewLocation();
        MotionControllerSourceSide == ESPointerSourceSide::Right ? RightController->DisablePointer() : LeftController->DisablePointer();
        PointerMotionSource = ESPointerSourceSide::NONE;
        bIsPointerActive = false;
    }
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

    if (WasHitMesh)
    {
        TraceResult = Result;
    }
    return WasHitMesh;
}


void ASVrPawn::TryToTeleportOnNewLocation()
{
    if (!bCanTeleport)
        return;
    const auto bValidLocationForTeleportWasFound = PointerMotionSource == ESPointerSourceSide::Right
                                                       ? RightController->GetValidLocationOnNavMeshWasFound()
                                                       : LeftController->GetValidLocationOnNavMeshWasFound();
    if (!bValidLocationForTeleportWasFound)
        return;
    if (UHeadMountedDisplayFunctionLibrary::GetHMDWornState() != EHMDWornState::Worn)
        return;

    const auto FoundedTeleportLocationOnNavMesh = PointerMotionSource == ESPointerSourceSide::Right
                                                      ? RightController->GetFoundedValidLocationOnNavMesh()
                                                      : LeftController->GetFoundedValidLocationOnNavMesh();
    FHitResult Result;
    if (const bool FindPointOnMeshUnderNavMesh = FindPointOnMeshFromNavMesh(FoundedTeleportLocationOnNavMesh, Result))
    {
        SetActorLocation(Result.Location);
    }

}
