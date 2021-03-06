#include "Pillar.h"
#include "Math/UnrealMathUtility.h"

#define OUT
#define NULLGUARD

// Sets default values for this component's properties
UPillar::UPillar()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPillar::SetAnswer(FString AnswerToSet, bool IsCorrect)
{
	Answer = AnswerToSet;
	CallGhostOnActivation = !IsCorrect;
	if (NULLGUARD TextComponent) TextComponent->SetText(FText::FromString(Answer));
}

FString UPillar::GetAnswer()
{
	return Answer;
}

bool UPillar::IsPillarActivated()
{
	return Activated;
}

// Called when the game starts
void UPillar::BeginPlay()
{
	Super::BeginPlay();
	TextComponent = GetOwner()->FindComponentByClass<UTextRenderComponent>();
	if(NULLGUARD ActorOnPillar) PrimitiveComponent = ActorOnPillar->FindComponentByClass<UPrimitiveComponent>();
	VerifyPointers();
	SetupPositions();
	SetupGhost();
	SetupSound();
	if (NULLGUARD TextComponent) TextComponent->SetText(FText::FromString(Answer));
}


void UPillar::SetupPositions()
{
	InitialPos = GetOwner()->GetActorLocation();
	CurrentPos = InitialPos;
	TargetPos = InitialPos;
	TargetPos.Z = InitialPos.Z - UnitsToLower;
	if (NULLGUARD !ActorOnPillar) return;
	InitialPosActorOnPillar = ActorOnPillar->GetActorLocation();
	InitialRotActorOnPillar = ActorOnPillar->GetActorRotation();
}

void UPillar::SetupGhost()
{
	TArray<AActor*> TempArrayOfGhost;
	FName Tag = TEXT("Ghost");
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, TempArrayOfGhost);
	if (TempArrayOfGhost.Num() > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s detected more then 1 'Ghost' in the map."), *(GetOwner()->GetName()));
		return;
	}
	if (TempArrayOfGhost.Num() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s detected no 'Ghost' in map."), *(GetOwner()->GetName()));
		return;
	}
	Ghost = TempArrayOfGhost[0]->FindComponentByClass<UGhost>();
	if (NULLGUARD !Ghost)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a refferense to 'Ghost'."), *(GetOwner()->GetName()));
		return;
	}
}

void UPillar::SetupSound()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (NULLGUARD !AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing component 'UAudioComponent'."), *(GetOwner()->GetName()));
	}
	if (WrongAnswerSounds.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing sounds."), *(GetOwner()->GetName()));
	}
	for (USoundBase* s : WrongAnswerSounds)
	{
		if (NULLGUARD !s)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a sound."), *(GetOwner()->GetName()));
		}
	}
	if (PillarSounds.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing sounds."), *(GetOwner()->GetName()));
	}
	for (USoundBase* s : PillarSounds)
	{
		if (NULLGUARD !s)
		{
			UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a sound."), *(GetOwner()->GetName()));
		}
	}
}

void UPillar::PlayWrongAnswerSound()
{
	if (NULLGUARD !AudioComponent || WrongAnswerSounds.Num() <= 0) return;
	int32 RandomNum = FMath::RandRange(0, WrongAnswerSounds.Num() - 1);
	if (NULLGUARD !WrongAnswerSounds[RandomNum])
	{
		AudioComponent->Play();
		return;
	}
	AudioComponent->SetSound(WrongAnswerSounds[RandomNum]);
	AudioComponent->Play();
}

void UPillar::PlayPillarSound()
{
	if (NULLGUARD !AudioComponent || PillarSounds.Num() <= 0) return;
	int32 RandomNum = FMath::RandRange(0, PillarSounds.Num() - 1);
	if (NULLGUARD !PillarSounds[RandomNum])
	{
		AudioComponent->Play();
		return;
	}
	AudioComponent->SetSound(PillarSounds[RandomNum]);
	AudioComponent->Play();
}

void UPillar::VerifyPointers()
{
	if (NULLGUARD !ActorOnPillar)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'ActorOnPillar'"), *(GetOwner()->GetName()));
	}
	if (NULLGUARD !TriggerVolume)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'TriggerVolume'"), *(GetOwner()->GetName()));
	}
	if (NULLGUARD !TextComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'TextComponent'"), *(GetOwner()->GetName()));
	}
	if (NULLGUARD !PrimitiveComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Actor %s is missing a 'PrimitiveComponent'"), *(GetOwner()->GetName()));
	}
}

void UPillar::ActivatePillar()
{
	if (NULLGUARD Ghost && CallGhostOnActivation)
	{
		Ghost->IncreaseSpeed();
		PlayWrongAnswerSound();
	}
	else PlayPillarSound();
	Activated = true;
	InProgress = true;
}

void UPillar::Progress(float DeltaTime)
{
	FVector ToMove = CurrentPos;
	CurrentPos = GetOwner()->GetActorLocation();
	ToMove.Z = FMath::FInterpTo(CurrentPos.Z, TargetPos.Z, DeltaTime, Speed);
	GetOwner()->SetActorLocation(ToMove);
	if (FMath::IsNearlyEqual(ToMove.Z, TargetPos.Z, 1.f))
	{
		InProgress = false;
		UE_LOG(LogTemp, Display, TEXT("Pillar done!"));
	}
}

void UPillar::Reset()
{
	//Resets
	GetOwner()->SetActorLocation(InitialPos);
	Activated = false;
	if (NULLGUARD ActorOnPillar && NULLGUARD PrimitiveComponent)
	{
		ActorOnPillar->GetRootComponent()->ComponentVelocity = FVector::ZeroVector;
		PrimitiveComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	}
	if (NULLGUARD ActorOnPillar) ActorOnPillar->SetActorLocation(InitialPosActorOnPillar);
	if (NULLGUARD ActorOnPillar) ActorOnPillar->SetActorRotation(InitialRotActorOnPillar);
}

// Called every frame
void UPillar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (
		NULLGUARD TriggerVolume &&
		NULLGUARD ActorOnPillar &&
		TriggerVolume->IsOverlappingActor(ActorOnPillar) == false &&
		Activated == false
		)
	{
		ActivatePillar();
	}

	if (Activated && InProgress && CallGhostOnActivation == false) Progress(DeltaTime);
}

