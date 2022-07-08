// Fill out your copyright notice in the Description page of Project Settings.


#include "HitCameraShake.h"

UHitCameraShake::UHitCameraShake()
{
	OscillationDuration = 0.3f;

	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.1f;

	LocOscillation.Y.Amplitude = 1.f;
	LocOscillation.Y.Frequency = 100.f;
	LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;


	LocOscillation.Z.Amplitude = 1.f;
	LocOscillation.Z.Frequency = 100.f;
	LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;

	bSingleInstance = true;
}