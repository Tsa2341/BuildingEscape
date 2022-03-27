// Tsa 2022 @ copyright


#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"

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
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	FVector PlayerViewPoint;
	FRotator PlayerRotation;



	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPoint,
		OUT PlayerRotation
	);

	/*UE_LOG(LogTemp, Warning, TEXT("Player viepoint = %s and  rotaion = %s"), 
		*PlayerViewPoint.ToString(), 
		*PlayerRotation.ToString()
	);*/

	// Shoe the the Line-Trace of the view
	FVector ViewLineEnd = PlayerViewPoint + PlayerRotation.Vector() * Reach;

	DrawDebugLine(
		GetWorld(),
		PlayerViewPoint,
		ViewLineEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	// Logging the hit object
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPoint,
		ViewLineEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	UE_LOG(LogTemp, Warning, TEXT(" Hit object is %s"), *(Hit.GetActor()->GetName()));

}

