/// \file	main.cpp
///	\date	2024/12/28
///	\author	Marc LE RENARD / FOKS Lab - Copyright 2024

#include "stdafx.h"
#include "Application.h"

CApplication * app = nullptr;

int main(int argc, char * argv[])
{
	app = CApplication::createInstance();
	app->run();

	return 1;
}