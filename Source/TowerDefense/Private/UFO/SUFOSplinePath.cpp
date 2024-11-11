
#include "UFO/SUFOSplinePath.h"
#include "Components/SplineComponent.h"

ASUFOSplinePath::ASUFOSplinePath()
{
 	PrimaryActorTick.bCanEverTick = false;

    Spline = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
    SetRootComponent(Spline);
}
