// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableElements/SMagicItem.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSMagicItem, All, All);

ASMagicItem::ASMagicItem()
{
    PrimaryActorTick.bCanEverTick = false;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    MagicTrackComponent = CreateDefaultSubobject<UNiagaraComponent>("MagicTrackComponent");
    MagicTrackComponent->SetupAttachment(StaticMeshComponent);
}

void ASMagicItem::PickUp(UMeshComponent* MeshForAttaching)
{
    UE_LOG(LogSMagicItem, Display, TEXT("Pickup"));
    if (MeshForAttaching)
    {
        StaticMeshComponent->SetSimulatePhysics(false);
        MagicTrackComponent->SetVisibility(true);
        const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        StaticMeshComponent->AttachToComponent(MeshForAttaching, AttachmentRules, SocketForAttaching);
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpSound, StaticMeshComponent->GetComponentLocation());
        bIsPickedUp = true;
    }
}

void ASMagicItem::Drop()
{
    const FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, false);
    DetachFromActor(Rules);
    StaticMeshComponent->SetSimulatePhysics(true);
    bIsPickedUp = false;
    MagicTrackComponent->SetVisibility(false);

}

FVector ASMagicItem::GetLocationInWorld()
{
    return StaticMeshComponent->GetComponentLocation();
}

FName ASMagicItem::GetNameForOrder()
{
    return TestName;
}

void ASMagicItem::BeginPlay()
{
    Super::BeginPlay();
    StartStateRelativeTransformOfStaticMesh = StaticMeshComponent->GetRelativeTransform();
    StaticMeshComponent->SetSimulatePhysics(true);
    MagicTrackComponent->SetVisibility(false);
}
