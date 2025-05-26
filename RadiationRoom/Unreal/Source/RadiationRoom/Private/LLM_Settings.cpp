// Fill out your copyright notice in the Description page of Project Settings.

#include "LLM_Settings.h"

ULLM_Settings::ULLM_Settings()
{
	scriptName = "llmSocket";
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
}

const FString ULLM_Settings::GetSettingsCommands() const
{
	FString args = TEXT("");
	for (auto command : LLM_Commands) {
		args += TEXT(" --") + command.CommandArgument + TEXT(" ") + command.CommandValue;
	}
	return args;
}

const FString ULLM_Settings::GetArgumentValue(FString argName) const
{
	for (auto arg : LLM_Commands) {
		if (arg.CommandArgument == argName) {
			return arg.CommandValue;
		}
	}
	return FString("");
}

const ULLM_Settings* ULLM_Settings::GetLLM_Settings()
{
	return GetDefault<ULLM_Settings>();
}
