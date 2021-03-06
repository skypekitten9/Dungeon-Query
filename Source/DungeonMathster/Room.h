// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Questions.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerBox.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/NameTypes.h"
#include "Ghost.h"
#include "Door.h"
#include "PlayerManager.h"
#include "Pillar.h"
#include "Components/TextRenderComponent.h"
#include "Room.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONMATHSTER_API URoom : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URoom();
	void Reset();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void SetupDoor();
	void SetupPlayer();
	void SetupAnswers();
	void SetupGhost();
	void VerifyTriggerVolume();
	void SetupPillars();
	void SetupTextComponent();
	bool IsCorrectPillarActivated();
	UPROPERTY(EditAnywhere) TArray<AActor*> PillarActors;
	UPROPERTY(EditAnywhere) ATriggerBox* TriggerVolume = nullptr;
	UPROPERTY(EditAnywhere) AActor* DoorActor = nullptr;
	UPROPERTY(EditAnywhere) float TimeBeforeReset = 1.f;
	UGhost* Ghost = nullptr;
	UTextRenderComponent* TextComponent = nullptr;
	FString CorrectAnswer = "";
	TArray<UPillar*> PillarComponents;
	UDoor* DoorComponent = nullptr;
	AActor* Player = nullptr;
	UPlayerManager* PlayerManager = nullptr;
	float ResetTimer = 0.f;
	bool HasReset = true;
	bool HasClosedDoor = false;
	bool PlayerPointRecieved = false;
};
