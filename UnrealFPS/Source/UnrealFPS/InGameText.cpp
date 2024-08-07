// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameText.h"

// Sets default values
AInGameText::AInGameText()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Text component
	MyText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("My text Component"));
	MyText->SetWorldSize(80.0f);
	MyText->SetText(FText::FromString("TextString"));

	RootComponent = MyText;
}

// Called when the game starts or when spawned
/**
 * Set the text to the text string
 */
void AInGameText::BeginPlay()
{
	Super::BeginPlay();
	MyText->SetText(FText::FromString(TextString));
}

/**
 * On a change in the editor, set the text to the text string
 * @param Transform 
 */
void AInGameText::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	MyText->SetText(FText::FromString(TextString));
}

// Called every frame
void AInGameText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
