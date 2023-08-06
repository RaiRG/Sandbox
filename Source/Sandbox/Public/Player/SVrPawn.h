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
class ASMotionController;
UCLASS()
class SANDBOX_API ASVrPawn : public APawn
{
    GENERATED_BODY()

public:
    ASVrPawn();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controller")
    TSubclassOf<ASMotionController> LeftMotionControllerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Controller")
    TSubclassOf<ASMotionController> RightMotionControllerClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="TeleportInfo | Settings",
        meta = (ToolTip=
            "Задает высоту луча, проведенного перпендикулярно вверх или вниз с точки на навигационном мэше, чтобый найти точку на мэше (например, земле)"
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
    
    //Teleport:
    bool FindPointOnMeshFromNavMesh(FVector LocationOnNavMesh, FHitResult& TraceResult) const;
    void TryToTeleportOnNewLocation();

    UFUNCTION()
    void OnMatchStateChanged(ESGameState GameState);

private:
    UPROPERTY()
    ASMotionController* LeftController;
    UPROPERTY()
    ASMotionController* RightController;
    
    bool bCanTeleport = false;

    bool bIsPointerActive;
    ESPointerSourceSide PointerMotionSource = ESPointerSourceSide::NONE; // на какой стороне сейчас активна указка
    
};
