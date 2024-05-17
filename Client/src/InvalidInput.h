#pragma once


enum class InvalidInput
{
	kNoErrors,
	kWrongLoginData,
	//input name errors======================
	kInvalidName,
	
	/*-----------------------------*/
	kEmptyName,
	kLongName,
	kWrongFirstNameChar,
	kWrongCharInsideName,
	kNameIsTaken,
	kAlreadyOnline,


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