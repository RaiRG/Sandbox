﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableElements/SMagicItem.h"
#include "NiagaraComponent.h"
#include "SMagicItemProperties.h"
#include "SObjectsHolder.h"
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

void ASMagicItem::PickUp(TScriptInterface<ISObjectsHolder> Holder, UMeshComponent* MeshForAttaching)
{
    UE_LOG(LogSMagicItem, Display, TEXT("Pickup"));
    if (!MeshForAttaching)
        return;

    if (bIsPickedUp && HolderObj)
    {
        HolderObj->ThrowOut();
        HolderObj = nullptr;
    }
    HolderObj = Holder;
    HolderObj->Hold(this);
    StaticMeshComponent->SetSimulatePhysics(false);
    MagicTrackComponent->SetVisibility(true);
    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
    StaticMeshComponent->AttachToComponent(MeshForAttaching, AttachmentRules, SocketForAttaching);
    if (Properties)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), Properties->PickUpSound, StaticMeshComponent->GetComponentLocation());
    }
    bIsPickedUp = true;
    bCanBePlacedOnTable = false;

}

void ASMagicItem::Drop()
{
    if (Properties)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), Properties->DropSound, StaticMeshComponent->GetComponentLocation());
    }
    HolderObj->ThrowOut();
    HolderObj = nullptr;
    const FDetachmentTransformRules Rules(EDetachmentRule::KeepWorld, false);
    DetachFromActor(Rules);
    StaticMeshComponent->SetSimulatePhysics(true);
    bIsPickedUp = false;
    MagicTrackComponent->SetVisibility(false);
    bCanBePlacedOnTable = true;
}

FVector ASMagicItem::GetLocationInWorld()
{
    return StaticMeshComponent->GetComponentLocation();
}


UTexture2D* ASMagicItem::GetIcon()
{
    if (!Properties)
        return nullptr;

    return Properties->Icon;
}

void ASMagicItem::BeginPlay()
{
    Super::BeginPlay();
    StartStateRelativeTransformOfStaticMesh = StaticMeshComponent->GetRelativeTransform();
    StaticMeshComponent->SetSimulatePhysics(true);
    MagicTrackComponent->SetVisibility(false);

    if (Properties)
    {
        MagicTrackComponent->SetColorParameter(VariableNameOfColor, Properties->MagicTrackColor);
        StaticMeshComponent->SetStaticMesh(Properties->Appearance);
    }
}
