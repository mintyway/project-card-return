// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include <chrono>

DECLARE_LOG_CATEGORY_EXTERN(PCRLogRuntime, Log, All);

#define SIMPLE_LOG \
{\
FString CallInformation = FString::Printf(TEXT("[%s](%d)"), *FString(__FUNCTION__), __LINE__);\
UE_LOG(PCRLogRuntime, Warning, TEXT("%s"), *CallInformation);\
}

#define RETURN_IF_INVALID(Expression, ...)\
if (!Expression)\
{\
FString CallInformation = FString::Printf(TEXT("[%s](%d)"), *FString(__FUNCTION__), __LINE__);\
FString ExpressionName = FString(#Expression);\
UE_LOG(PCRLogRuntime, Error, TEXT("%s Assertion: %s"), *CallInformation, *ExpressionName);\
return __VA_ARGS__;\
}

#define NULL_POINTER_EXCEPTION(NullPointerVariable)\
{\
FString CallInformation = FString::Printf(TEXT("[%s](%d)"), *FString(__FUNCTION__), __LINE__);\
FString PointerVariableName = FString(#NullPointerVariable);\
UE_LOG(PCRLogRuntime, Error, TEXT("%s NullPointerException: %s"), *CallInformation, *PointerVariableName);\
}

#define TIME_CHECK_START(Number) auto Macro_TimeCheckStart##Number = std::chrono::high_resolution_clock::now();

#define TIME_CHECK_END(Number)\
auto Macro_TimeCheckEnd##Number = std::chrono::high_resolution_clock::now();\
auto Macro_TimeCheckDuration##Number = std::chrono::duration_cast<std::chrono::nanoseconds>(Macro_TimeCheckEnd##Number - Macro_TimeCheckStart##Number);\
auto Macro_TimeCheckDurationToDouble##Number = static_cast<double>(Macro_TimeCheckDuration##Number.count());\
UE_LOG(PCRLogRuntime, Warning, TEXT("Timer %d: %.6fms\t %.3fμs\t %.0fns"), Number, Macro_TimeCheckDurationToDouble##Number / 1000000, Macro_TimeCheckDurationToDouble##Number / 1000, Macro_TimeCheckDurationToDouble##Number);