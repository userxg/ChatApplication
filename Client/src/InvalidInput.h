#pragma once


enum class InvalidInput
{
	NoErrors,

	//input name errors======================
	kInvalidName,
	
	/*-----------------------------*/
	kEmptyName,
	kLongName,
	kWrongFirstNameChar,
	kWrongCharInsideName,


	//input password errors======================
	kInvalidPassword,

	/*----------------------------------------*/
	kRePasswordIsNotEqual,
	kEmptyPassword,
	kLongPassword,
	kWrongCharInsidePassword
};

struct InputError
{
	//area - name or password
	InvalidInput area;
	//type
	InvalidInput type;
};