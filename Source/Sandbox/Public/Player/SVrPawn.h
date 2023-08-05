// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "SCoreTypes.h"
#include "SVrPawn.generated.h"


class UArrowComponent;
class UMotionControllerComponent;
class UCameraComponent;
class UNiagaraComponent;
class UInputMappingContext;
class UInputAction;
class UWidgetInteractionComponent;

UCLASS()
class SANDBOX_API ASVrPawn : public APawn
{
    GENERATED_BODY()

public:
    ASVrPawn();

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
    float PointerMaxDistance = 350.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="PoitnerSettings")
    FString PropertyForDistanceAdjusting = "User.BeamEnd";

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
    float TurningDegrees = 30.0f;

    void TurnPlayer(const FInputActionValue& Value);
    void SnapTurn(bool IsRightTurn, float Degrees);
    void InputData(const FName MotionControllerSourceSide);
    
    void DisablePointer(const FName MotionControllerSourceSide);
    void ActivatePointer(const FName MotionControllerSourceSide);
    void AttachPointerToController(FName MotionControllerSourceSide, UMeshComponent* ControllerMesh);
    void UpdatePointerTrace();
    
    void OnMatchStateChanged(ESTGameState GameState);
private:
    bool bIsPointerActive;
    FName PointerMotionSource; // на какой стороне сейчас активна указка

    bool bCanTeleport = false;
};
