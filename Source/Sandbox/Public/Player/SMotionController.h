// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SObjectsHolder.h"
#include "../../../../../../EpicGames/UE_5.2/Engine/Platforms/Hololens/Source/Runtime/Core/Public/Microsoft/AllowMicrosoftPlatformTypesPrivate.h"
#include "GameFramework/Actor.h"
#include "SMotionController.generated.h"

class UArrowComponent;
class UMotionControllerComponent;
class UCameraComponent;
class UNiagaraComponent;
class UInputMappingContext;
class UInputAction;
class UWidgetInteractionComponent;

UCLASS()
class SANDBOX_API ASMotionController : public AActor, public ISObjectsHolder
{
    GENERATED_BODY()

public:
    ASMotionController();

    bool GetValidLocationOnNavMeshWasFound() { return bValidLocationOnNavMeshWasFound; }
    FVector GetFoundedValidLocationOnNavMesh() { return FoundedValidLocationOnNavMesh; }

    virtual bool GetDoesHold() {return bDoesHold; };
    virtual void Hold(AActor* ObjectForHolding)  override;
    virtual void ThrowOut() override;
    
    void DisablePointer();
    void ActivatePointer();
    bool IsValidNavLocation(FVector& ProjectedLocationOnNavMesh, FVector HitLocation) const;
    void InputData() const;
    void InteractWithWorld();
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UMotionControllerComponent* MotionController;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UNiagaraComponent* PointerTrace;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UWidgetInteractionComponent* WidgetInteraction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PointerSettings")
    FName PointerSocketName = "PointerSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PointerSettings")
    float PointerMaxDistance = 700.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PointerSettings")
    FName PropertyForDistanceAdjusting = "User.BeamEnd";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PointerSettings")
    FName PropertyForColorAdjusting = "User.PointerColor";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PointerSettings | Teleport Info")
    FLinearColor ValidHitColor = FLinearColor::Green;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PointerSettings | Teleport Info")
    FLinearColor InvalidHitColor = FLinearColor::Red;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PointerSettings | Teleport Info",
        meta = (ToolTip=
            "Точность нахождения точки на навигационном мэше. Как далеко в каждом направлении искать точку на нав меше, прежде чем сдаться"
        ))
    FVector QueryingExtent = FVector(0.0f, 0.0f, 0.0f);

private:
    //Pointer
    void UpdatePointerTrace();
    bool bIsPointerActive;

    // Location on NavMesh
    bool bValidLocationOnNavMeshWasFound = false;
    FVector FoundedValidLocationOnNavMesh;

    //Holder:
    bool bDoesHold = false;
    
    UPROPERTY()
    AActor* HoldedObject = nullptr;

};
