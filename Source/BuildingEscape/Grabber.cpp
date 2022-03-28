// Tsa 2022 @ copyright


#include "Grabber.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UGrabber::GetPhysicsHandleComponent();
	UGrabber::BindInputAction();
	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbed"));

	auto ActorComponent = GetHit().GetComponent();
	AActor* ActorHit = GetHit().GetActor();

	if (ActorHit) {
		PhysicsHandle->GrabComponent(
			ActorComponent,
			NAME_None,
			ActorComponent->GetOwner()->GetActorLocation(),
			true // allow rotaion
		);
	}
}

void UGrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("Released"));
	PhysicsHandle->ReleaseComponent();
}

// Sets the physics handle component
void UGrabber::GetPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT(" %s doesn't have a Physics HAndle Component attached"), *(GetOwner()->GetName()));
	}
}

// gets the Input component and Binds Input action
void UGrabber::BindInputAction()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		// Bind input Action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT(" %s doesn't have a Input Component attached"), *(GetOwner()->GetName()));
	}
}

// gets the playerViewLocation and his rotaion and use them to create a Trace-Line end
void UGrabber::SetViewLineEnd()
{
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	PlayerViewLineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

}

FHitResult UGrabber::GetHit()
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());


	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		PlayerViewLineEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT(" Line-trace hit %s"), *(ActorHit->GetName()));
	}

	return Hit;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UGrabber::SetViewLineEnd();

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		PlayerViewLineEnd,
		FColor(255, 0, 0),
		false, 0.f, 0.f, 10.f
	);

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(PlayerViewLineEnd);
	}
}

