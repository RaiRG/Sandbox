// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableElements/SMagicItem.h"

DEFINE_LOG_CATEGORY_STATIC(LogSMagicItem, All, All);

ASMagicItem::ASMagicItem()
{
    PrimaryActorTick.bCanEverTick = false;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

}

void ASMagicItem::PickUp(UMeshComponent* MeshForAttaching)
{
    UE_LOG(LogSMagicItem, Display, TEXT("Pickup"));
    if (MeshForAttaching)
    {
        UE_LOG(LogSMagicItem, Display, TEXT("Actor Location 1 %s"), *GetActorLocation().ToString());
        StaticMeshComponent->SetSimulatePhysics(false);
        const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
        StaticMeshComponent->AttachToComponent(MeshForAttaching, AttachmentRules, SocketForAttaching);
        bIsPickedUp = true;
    }
}

void ASMagicItem::Drop()
{
    const FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, false);
    DetachFromActor(Rules);
    StaticMeshComponent->SetSimulatePhysics(true);
    bIsPickedUp = false;
    UE_LOG(LogSMagicItem, Display, TEXT("Actor Location 2 %s"), *GetActorLocation().ToString());

}

void ASMagicItem::BeginPlay()
{
    Super::BeginPlay();
    StartStateRelativeTransformOfStaticMesh = StaticMeshComponent->GetRelativeTransform();
    StaticMeshComponent->SetSimulatePhysics(true);
}
