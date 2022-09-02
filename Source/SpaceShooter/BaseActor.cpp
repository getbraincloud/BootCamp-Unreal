// Copyright 2022 bitHeads, Inc. All Rights Reserved.


#include "BaseActor.h"
#include "SpaceShooterGameModeBase.h"
#include "Spawner.h"
#include "PaperSprite.h"


// Sets default values
ABaseActor::ABaseActor() :
    m_SpriteSize(FVector2D::ZeroVector),
    m_Depth(1.0f),
    m_Radians(0.0f),
    m_Health(1),
    m_AttackDamage(1),
    m_IsActive(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    m_Root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(m_Root);
    
    m_Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
    m_Sprite->SetSimulatePhysics(false);
    
    m_Sprite->GetBodyInstance()->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
    m_Sprite->GetBodyInstance()->SetObjectType( ECollisionChannel::ECC_WorldStatic );
    m_Sprite->GetBodyInstance()->SetResponseToChannel( ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap );

    m_Sprite->SetupAttachment(m_Root);
}

// Called when the game starts or when spawned
void ABaseActor::BeginPlay()
{
	Super::BeginPlay();
    
    UPaperSprite* sprite = m_Sprite->GetSprite();
    if(sprite != nullptr)
        m_SpriteSize = FVector2D(sprite->GetBakedTexture()->GetSizeX(), sprite->GetBakedTexture()->GetSizeY());
}

// Called every frame
void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    float timeScale = GetGameMode()->GetTimeScale();

    if(m_Fader.IsFading())
    {
        m_Fader.Tick(DeltaTime * timeScale);
        SetAlpha(m_Fader.GetAlpha());
    }
}

void ABaseActor::SetPosition(FVector2D position)
{
    RootComponent->SetRelativeLocation(FVector(position.X, m_Depth, position.Y));
}

FVector2D ABaseActor::GetPosition() const
{
    FVector position = RootComponent->GetRelativeLocation();
    return FVector2D(position.X, position.Z);
}

void ABaseActor::SetScale(FVector2D scale)
{
    RootComponent->SetRelativeScale3D(FVector(scale.X, 1.0f, scale.Y));
}

FVector2D ABaseActor::GetScale() const
{
    FVector scale = RootComponent->GetRelativeScale3D();
    return FVector2D(scale.X, scale.Z);
}

void ABaseActor::SetDepth(float depth)
{
    m_Depth = depth;
    
    // Update RootComponent's position
    FVector position = RootComponent->GetRelativeLocation();
    RootComponent->SetRelativeLocation(FVector(position.X, m_Depth, position.Z));
}

float ABaseActor::GetDepth() const
{
    return m_Depth;
}

void ABaseActor::SetRadians(float radians)
{
    m_Radians = radians;
    RootComponent->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0.0f, FMath::RadiansToDegrees(m_Radians), 0.0f)).Rotator());
}

float ABaseActor::GetRadians() const
{
    return m_Radians;
}

FVector2D ABaseActor::Translate(FVector2D displacement)
{
    FVector2D position = GetPosition();
    position += displacement;
    SetPosition(position);
    
    return position;
}

bool ABaseActor::ApplyDamage(int damage)
{
    m_Health -= damage;
    
    if(m_Health <= 0)
        m_Health = 0;
    
    return m_Health == 0;
}

void ABaseActor::SetHealth(int health)
{
    m_Health = health;
}

int ABaseActor::GetHealth() const
{
    return m_Health;
}

void ABaseActor::SetAttackDamage(int attackDamange)
{
    m_AttackDamage = attackDamange;
}

int ABaseActor::GetAttackDamage() const
{
    return m_AttackDamage;
}

void ABaseActor::Activate()
{
    m_IsActive = true;
    
    SetActorHiddenInGame(!m_IsActive);
    SetActorEnableCollision(m_IsActive);
    SetActorTickEnabled(m_IsActive);
}

void ABaseActor::Deactivate()
{
    m_IsActive = false;
    
    SetActorHiddenInGame(!m_IsActive);
    SetActorEnableCollision(m_IsActive);
    SetActorTickEnabled(m_IsActive);
}

bool ABaseActor::IsActive() const
{
    return m_IsActive;
}

FVector2D ABaseActor::GetSpriteSize() const
{
    return m_SpriteSize;
}

void ABaseActor::SetAlpha(float alpha)
{
    FLinearColor spriteColor = m_Sprite->GetSpriteColor();
    spriteColor.A = alpha;
    m_Sprite->SetSpriteColor(spriteColor);
}

float ABaseActor::GetAlpha() const
{
    FLinearColor spriteColor = m_Sprite->GetSpriteColor();
    return spriteColor.A;
}

void ABaseActor::FadeOut(float duration, float delay)
{
    m_Fader.StartFade(Fader::Type::Out, duration, delay, this);
}

bool ABaseActor::IsFadingOut() const
{
    return m_Fader.IsFadingOut();
}

void ABaseActor::FadeIn(float duration, float delay)
{
    m_Fader.StartFade(Fader::Type::In, duration, delay, this);
}

bool ABaseActor::IsFadingIn() const
{
    return m_Fader.IsFadingIn();
}

float ABaseActor::GetTimeScale()
{
    return GetGameMode()->GetTimeScale();
}

void ABaseActor::SetSprite(const FString& texture)
{
    FString path = "/Game/Sprites/" + texture + "_Sprite";
    UPaperSprite* sprite = Cast<UPaperSprite>(StaticLoadObject(UPaperSprite::StaticClass(), nullptr, *path));
    
    if(sprite != nullptr)
    {
        m_Sprite->SetSprite(sprite);
        m_SpriteSize = FVector2D(sprite->GetBakedTexture()->GetSizeX(), sprite->GetBakedTexture()->GetSizeY());
    }
}

ASpaceShooterGameModeBase* ABaseActor::GetGameMode()
{
    ASpaceShooterGameModeBase* gameMode = Cast<ASpaceShooterGameModeBase>(GetWorld()->GetAuthGameMode());
    check(gameMode != nullptr);
    
    return gameMode;
}

USpawner* ABaseActor::GetSpawner()
{
    USpawner* spawner = GetGameMode()->GetSpawner();
    check(spawner != nullptr);
    
    return spawner;
}

void ABaseActor::ResetFade()
{
    m_Fader.Reset();
    SetAlpha(1.0f);
}

void ABaseActor::OnFadeOutHasCompleted(Fader* fader)
{
    if(fader == &m_Fader)
    {
        ResetFade();
        Deactivate();
    }
}
