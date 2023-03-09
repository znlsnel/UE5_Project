// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTPSProject_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_TPSProject;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_TPSProject()
	{
		if (!Z_Registration_Info_UPackage__Script_TPSProject.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/TPSProject",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x02252F20,
				0xD38109B9,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_TPSProject.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_TPSProject.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_TPSProject(Z_Construct_UPackage__Script_TPSProject, TEXT("/Script/TPSProject"), Z_Registration_Info_UPackage__Script_TPSProject, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x02252F20, 0xD38109B9));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
