// Fill out your copyright notice in the Description page of Project Settings.

#include "LLM_Settings.h"
#include "ISettingsModule.h"

ULLM_Settings::ULLM_Settings(const FObjectInitializer& obj)
{
	/*ExitCommand = TEXT("LLM_Quit");
	HostPort = 8080;
	HostIP = TEXT("127.0.0.1");
	HistoryMaxMemory = 0;
	PerformanceMode = TEXT("Fast");
	ExecutionMode = TEXT("Release");*/

	FCommand exitCom; exitCom.CommandArgument = "exit_msg"; exitCom.CommandValue = "LLM_Quit";
	LLM_Commands.Add(exitCom);
	FCommand portCom; portCom.CommandArgument = "port"; portCom.CommandValue = "8080";
	LLM_Commands.Add(portCom);
	FCommand IPCom; IPCom.CommandArgument = "host_ip"; IPCom.CommandValue = "127.0.0.1";
	LLM_Commands.Add(IPCom);
	FCommand HistCom; HistCom.CommandArgument = "hist_max_mem"; HistCom.CommandValue = "0";
	LLM_Commands.Add(HistCom);
	FCommand PerfCom; PerfCom.CommandArgument = "perf_mode"; PerfCom.CommandValue = "Fast";
	LLM_Commands.Add(PerfCom);
	FCommand ExeCom; ExeCom.CommandArgument = "exe_mode"; ExeCom.CommandValue = "Debug";
	LLM_Commands.Add(ExeCom);
	FCommand TempCom; TempCom.CommandArgument = "temperature"; TempCom.CommandValue = "75";
	LLM_Commands.Add(TempCom);

	//https://forums.unrealengine.com/t/how-to-add-custom-settings-in-project-settings/146330/3
	//Registro de los valores en el apartado de ajustes del proyecto
	/*if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "Source", "LLM_Settings",
			LOCTEXT("RuntimeSettingsName", "LLM settings configuration"),
			LOCTEXT("RuntimeSettingsDescription", "Configure the settings related to the LLM execution"),
			GetMutableDefault<ULLM_Settings>()
		);
	}

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "MySetting");
	}*/
}

const FString ULLM_Settings::GetSettingsCommands()
{
	FString args = TEXT("");
	for (auto command : LLM_Commands) {
		args += TEXT(" --") + command.CommandArgument + TEXT(" ") + command.CommandValue;
	}
	return args;
}
