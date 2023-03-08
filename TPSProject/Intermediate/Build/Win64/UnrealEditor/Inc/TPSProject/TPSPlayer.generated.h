// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef TPSPROJECT_TPSPlayer_generated_h
#error "TPSPlayer.generated.h already included, missing '#pragma once' in TPSPlayer.h"
#endif
#define TPSPROJECT_TPSPlayer_generated_h

#define FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_SPARSE_DATA
#define FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_RPC_WRAPPERS
#define FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_RPC_WRAPPERS_NO_PURE_DECLS
#define FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesATPSPlayer(); \
	friend struct Z_Construct_UClass_ATPSPlayer_Statics; \
public: \
	DECLARE_CLASS(ATPSPlayer, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/TPSProject"), NO_API) \
	DECLARE_SERIALIZER(ATPSPlayer)


#define FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_INCLASS \
private: \
	static void StaticRegisterNativesATPSPlayer(); \
	friend struct Z_Construct_UClass_ATPSPlayer_Statics; \
public: \
	DECLARE_CLASS(ATPSPlayer, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/TPSProject"), NO_API) \
	DECLARE_SERIALIZER(ATPSPlayer)


#define FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ATPSPlayer(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ATPSPlayer) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ATPSPlayer); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ATPSPlayer); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ATPSPlayer(ATPSPlayer&&); \
	NO_API ATPSPlayer(const ATPSPlayer&); \
public:


#define FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ATPSPlayer(ATPSPlayer&&); \
	NO_API ATPSPlayer(const ATPSPlayer&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ATPSPlayer); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ATPSPlayer); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ATPSPlayer)


#define FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_9_PROLOG
#define FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_SPARSE_DATA \
	FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_RPC_WRAPPERS \
	FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_INCLASS \
	FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_SPARSE_DATA \
	FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_INCLASS_NO_PURE_DECLS \
	FID_TPSProject_Source_TPSProject_public_TPSPlayer_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> TPSPROJECT_API UClass* StaticClass<class ATPSPlayer>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_TPSProject_Source_TPSProject_public_TPSPlayer_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
