// Fill out your copyright notice in the Description page of Project Settings.


#include "SMotionController.h"

#include "MotionControllerComponent.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "SPickableUp.h"
#include "Components/WidgetInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


ASMotionController::ASMotionController()
{
    PrimaryActorTick.bCanEverTick = true;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    MotionController = CreateDefaultSubobject<UMotionControllerComponent>("MotionControllerRight");
    MotionController->SetupAttachment(GetRootComponent());

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponentLeft");
    StaticMeshComponent->SetupAttachment(MotionController);

    PointerTrace = CreateDefaultSubobject<UNiagaraComponent>("PointerTrace");
    PointerTrace->SetupAttachment(GetRootComponent());

    WidgetInteraction = CreateDefaultSubobject<UWidgetInteractionComponent>("WidgetInteraction");
    WidgetInteraction->SetupAttachment(PointerTrace);
    WidgetInteraction->InteractionDistance = 0.0f;
}

void ASMotionController::BeginPlay()
{
    Super::BeginPlay();

    if (PointerTrace)
    {
        PointerTrace->SetVisibility(false);
        PointerTrace->SetVectorParameter(PropertyForDistanceAdjusting, FVector(PointerMaxDistance, 0.0f, 0.0f));
    }

}

void ASMotionController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsPointerActive)
    {
        UpdatePointerTrace();
    }
}

void ASMotionController::UpdatePointerTrace()
{
    if (bIsPointerActive)
    {
        FVector SocketLocation;
        FRotator SocketRotation;

        StaticMeshComponent->GetSocketWorldLocationAndRotation(PointerSocketName, SocketLocation,
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
            bValidLocationOnNavMeshWasFound = IsValidNavLocation(FoundedValidLocationOnNavMesh, HitResult.Location);
        }

        if (bValidLocationOnNavMeshWasFound)
        {
            PointerTrace->SetColorParameter(PropertyForColorAdjusting, ValidHitColor);
        }
        else
        {
            FoundedValidLocationOnNavMesh = FVector();
            PointerTrace->SetColorParameter(PropertyForColorAdjusting, InvalidHitColor);
        }
    }
}

void ASMotionController::Hold(AActor* ObjectForHolding)
{
    if (bDoesHold)
        return;
    bDoesHold = true;
    HoldedObject = ObjectForHolding;
}

void ASMotionController::ThrowOut()
{
    bDoesHold = false;
    HoldedObject = nullptr;
}

bool ASMotionController::IsValidNavLocation(FVector& ProjectedLocationOnNavMesh, FVector HitLocation) const
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

void ASMotionController::InputData() const
{
    if (!WidgetInteraction || !bIsPointerActive)
        return;
    WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
    WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
}

void ASMotionController::InteractWithWorld()
{
    if (bDoesHold)
    {
        if (UKismetSystemLibrary::DoesImplementInterface(HoldedObject, USPickableUp::StaticClass()))
        {
            if (TScriptInterface<ISPickableUp> ActorForDrop =TScriptInterface<ISPickableUp>(HoldedObject))
            {
                ActorForDrop->Drop();
            }
        }
    }
    else if (!bDoesHold)
    {
        TArray<AActor*> OverlappingActors;
        StaticMeshComponent->GetOverlappingActors(OverlappingActors, AActor::StaticClass());
        for (auto OverlappingActor : OverlappingActors)
        {
            if (UKismetSystemLibrary::DoesImplementInterface(OverlappingActor, USPickableUp::StaticClass()))
            {
                if (TScriptInterface<ISPickableUp> ActorForPicking =TScriptInterface<ISPickableUp>(OverlappingActor))
                {
                    ActorForPicking->PickUp(this, StaticMeshComponent);
                    break;
                }
            }
        }
    }
}

void ASMotionController::DisablePointer()
{
    PointerTrace->SetVisibility(false);
    bIsPointerActive = false;
    WidgetInteraction->InteractionDistance = 0.0f;
}

void ASMotionController::ActivatePointer()
{
    if (bIsPointerActive || bDoesHold)
        return;
    PointerTrace->SetVisibility(true);
    WidgetInteraction->InteractionDistance = PointerMaxDistance;
    const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
    PointerTrace->AttachToComponent(StaticMeshComponent, AttachmentTransformRules, PointerSocketName);
    bIsPointerActive = true;
}
