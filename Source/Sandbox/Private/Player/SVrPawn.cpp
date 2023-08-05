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
    }

    auto GameMode = GetWorld() ? Cast<ASGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
    if (GameMode)
    {
        GameMode->OnMatchStateChanged.AddUObject(this, &ASVrPawn::OnMatchStateChanged);
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
            &ASVrPawn::ActivatePointer, FName("Right"));
        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnLeft, ETriggerEvent::Started, this,
            &ASVrPawn::ActivatePointer, FName("Left"));

        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnRight, ETriggerEvent::Triggered, this,
           &ASVrPawn::UpdatePointerTrace);
        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnLeft, ETriggerEvent::Triggered, this,
            &ASVrPawn::UpdatePointerTrace);
        
        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnRight, ETriggerEvent::Completed, this,
            &ASVrPawn::DisablePointer, FName("Right"));
        PlayerEnhancedInputComponent->BindAction(TeleportTraceOnLeft, ETriggerEvent::Completed, this,
            &ASVrPawn::DisablePointer, FName("Left"));

        //WidgetInteraction
        PlayerEnhancedInputComponent->BindAction(TriggerClickOnRight, ETriggerEvent::Started, this,
            &ASVrPawn::InputData, FName("Right"));
        PlayerEnhancedInputComponent->BindAction(TriggerClickOnLeft, ETriggerEvent::Started, this,
            &ASVrPawn::InputData, FName("Left"));
    }
}

void ASVrPawn::TurnPlayer(const FInputActionValue& Value)
{
    UE_LOG(LogSVrPawn, Display, TEXT("TurnPlayer"));
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

void ASVrPawn::ActivatePointer(const FName MotionControllerSourceSide)
{
    if (!bIsPointerActive)
    {
        if (MotionControllerRight && MotionControllerRight->MotionSource == MotionControllerSourceSide)
        {
            AttachPointerToController(MotionControllerSourceSide, StaticMeshComponentRight);
        }
        else if (MotionControllerLeft && MotionControllerLeft->MotionSource == MotionControllerSourceSide)
        {
            AttachPointerToController(MotionControllerSourceSide, StaticMeshComponentLeft);
        }
    }
}

void ASVrPawn::AttachPointerToController(FName MotionControllerSourceSide, UMeshComponent* ControllerMesh)
{
    PointerTrace->SetVisibility(true);
    WidgetInteraction->InteractionDistance = PointerMaxDistance;
    FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
    PointerTrace->AttachToComponent(ControllerMesh, AttachmentTransformRules, PointerSocketName);
    PointerMotionSource = MotionControllerSourceSide;
    bIsPointerActive = true;
}

void ASVrPawn::InputData(const FName MotionControllerSourceSide)
{
    if (!WidgetInteraction || !bIsPointerActive || PointerMotionSource != MotionControllerSourceSide)
        return;
    UE_LOG(LogSVrPawn, Display, TEXT("TriggerClick"));
    WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
    WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

void ASVrPawn::OnMatchStateChanged(ESTGameState GameState)
{
    if (GameState == ESTGameState::InProgress)
    {
        bCanTeleport = true;
    }
    else
    {
        bCanTeleport = false;
    }
}

void ASVrPawn::DisablePointer(const FName MotionControllerSourceSide)
{
    UE_LOG(LogSVrPawn, Display, TEXT("HidePointer!"));
    if (PointerMotionSource == MotionControllerSourceSide)
    {
        PointerTrace->SetVisibility(false);
        PointerMotionSource = FName();
        bIsPointerActive = false;
        WidgetInteraction->InteractionDistance = 0.0f;
    }
}

void ASVrPawn::UpdatePointerTrace()
{
    UE_LOG(LogSVrPawn, Display, TEXT("UpdatePointerTrace"));

    if (bCanTeleport && bIsPointerActive && PointerTrace)
    {
        //Смена цвета
    }
}

