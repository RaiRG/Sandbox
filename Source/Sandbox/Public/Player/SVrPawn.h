// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "SCoreTypes.h"
#include "SObjectsHolder.h"
#include "SVrPawn.generated.h"


class UArrowComponent;
class UMotionControllerComponent;
class UCameraComponent;
class UNiagaraComponent;
class UInputMappingContext;
class UInputAction;
class UWidgetInteractionComponent;

UCLASS()
class SANDBOX_API ASVrPawn : public APawn , public  ISObjectsHolder
{
    GENERATED_BODY()

public:
    ASVrPawn();

    virtual bool GetDoesHold() {return bDoesHold; };
protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UStaticMeshComponent* StaticMeshComponentLeft;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UStaticMeshComponent* StaticMeshComponentRight;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UMotionControllerComponent* MotionControllerLeft;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UMotionControllerComponent* MotionControllerRight;

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
    FVector QueryingExtent = FVector(300.0f, 300.0f, 300.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TeleportInfo | Settings",
        meta = (ToolTip=
            "Задает высоту луча, проведенного перпендикулярно вверх или вниз с точки на навигационном мэше, чтобый найти точку на мэше (например, земле)200"
        ))
    float HeightOfVerticalLineFromNavMesh = 1000.0f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Inputs | IMC")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Inputs | IA")
    UInputAction* ThumbstickTurning;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Inputs | IA")
    UInputAction* TeleportTraceOnLeft;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Inputs | IA")
    UInputAction* TeleportTraceOnRight;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Inputs | IA")
    UInputAction* TriggerClickOnRight;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Inputs | IA")
    UInputAction* TriggerClickOnLeft;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Inputs | IA")
    UInputAction* GrabClickOnRight;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Inputs | IA")
    UInputAction* GrabClickOnLeft;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Inputs | IA")
    float TurningDegrees = 30.0f;

    //Change stet of actor
    void TurnPlayer(const FInputActionValue& Value);
    void SnapTurn(bool IsRightTurn, float Degrees);

    //Interaction with world:
    void InputData(const ESPointerSourceSide MotionControllerSourceSide);
    void TryToInteractWithWorld(const ESPointerSourceSide MotionControllerSourceSide);
    
    //Pointer:
    void DisablePointer(const ESPointerSourceSide MotionControllerSourceSide);
    void ActivatePointer(const ESPointerSourceSide MotionControllerSourceSide);
    void AttachPointerToController(ESPointerSourceSide MotionControllerSourceSide, UMeshComponent* ControllerMesh);
    void UpdatePointerTrace();

    //Teleport:
    bool IsValidTeleportLocation(FVector& ProjectedLocationOnNavMesh, FVector HitLocation) const;
    bool FindPointOnMeshFromNavMesh(FVector LocationOnNavMesh, FHitResult& TraceResult) const;
    void TryToTeleportOnNewLocation();

    UFUNCTION()
    void OnMatchStateChanged(ESGameState GameState);

private:
    bool bCanTeleport = false;

    bool bIsPointerActive;
    ESPointerSourceSide PointerMotionSource = ESPointerSourceSide::NONE; // на какой стороне сейчас активна указка

    bool bValidLocationForTeleportWasFound = false;
    FVector FoundedTeleportLocationOnNavMesh;
    
    virtual void  Hold(AActor* ObjectForHolding,  USceneComponent* HoldingComponent)  override;
    virtual void ThrowOut() override;
    
    bool bDoesHold = false;
    ESPointerSourceSide HolderSide = ESPointerSourceSide::NONE;
    
    UPROPERTY()
    AActor* HoldedObject = nullptr;

};
