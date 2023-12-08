#include <windows.h>
#include <fstream>
#include "resource.h"
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <Winuser.h>

using namespace std;

struct veterinario
{
	char NomU[50], contrasena[30], NOMBREV[50],CEDULA[15], FOTO[256];
	veterinario* sigV;
	veterinario* anteV;
};
veterinario* inicioV, *AuxV = nullptr;

struct cita
{
	char NomC[50], TEL[11], Especie[30], NomM[50], Fecha[6], Hora[6], Motivo[100], Precio[10], MES[6];
	int Estado=0;
	cita* sigC;
	cita* anteC;
};
cita* inicioC, * AuxC = nullptr;



veterinario* BOT = nullptr;
BOOL CALLBACK veterinarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void menu(WPARAM wParam, HWND hwnd);
void REGAUSU(veterinario* nuevo);
void leerMedico();
void escribirMedico();
BOOL CALLBACK INICIOSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK PRINCIPAL(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CITAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void registrarCit(cita* nuevo);
void leerCitas();
void escribirCitas();
BOOL CALLBACK AGENDA(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ELIMINAR(char Eliminar[50], HWND hwnd);
void MODIFICAR(char Mod[], HWND hwnd, cita* nuevo);
BOOL CALLBACK INFOMED(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ELIMINARMED(char Eliminar[50], HWND hwnd);
void MODIFICARMED(char Mod[], HWND hwnd, veterinario* nuevo);

HINSTANCE HISNG;
char zFile[MAX_PATH];
bool IMAGE = 0;

int WINAPI WinMain(HINSTANCE HINST, HINSTANCE HIPrev, PSTR cmdline, int cmdshow)
{
	leerMedico();
	leerCitas();

	HISNG = HINST;

	HWND hwregistrousu;

	hwregistrousu = CreateDialog(HINST, MAKEINTRESOURCE(IDD_DIALOG3), NULL, INICIOSesion);
	ShowWindow(hwregistrousu, cmdshow);

	MSG mensajes;
	ZeroMemory(&mensajes, sizeof(mensajes));

	while (GetMessage(&mensajes, 0, 0, 0))
	{
		TranslateMessage(&mensajes);
		DispatchMessage(&mensajes);
	}

	escribirMedico();
	escribirCitas();

	return 0;

	
}

BOOL CALLBACK veterinarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc)
		{
		case IDYES:
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}break;
		}
	}break;

	case WM_INITDIALOG:
	{
		 IMAGE = 0;

		if (BOT == nullptr)
		{
			EnableMenuItem(GetMenu(hwnd), ID_REGISTROS_USUARIOS, MF_DISABLED | MF_BYCOMMAND);
			DeleteMenu(GetMenu(hwnd), ID_REGISTROS_USUARIOS, MF_BYCOMMAND);

			EnableMenuItem(GetMenu(hwnd), ID_REGISTROS_CITAS, MF_DISABLED | MF_BYCOMMAND);
			DeleteMenu(GetMenu(hwnd), ID_REGISTROS_CITAS, MF_BYCOMMAND);

			EnableMenuItem(GetMenu(hwnd), ID_MANEJODECITAS_AGENDA, MF_DISABLED | MF_BYCOMMAND);
			DeleteMenu(GetMenu(hwnd), ID_MANEJODECITAS_AGENDA, MF_BYCOMMAND);


		}
		else
		{
			SetDlgItemText(hwnd, IDC_EDIT1, BOT->NOMBREV);

			HBITMAP bmp;

			bmp = (HBITMAP)LoadImage(NULL, BOT->FOTO, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);

			SendDlgItemMessage(hwnd, IDC_FOTOIS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		}


	}break;

	case WM_COMMAND:
	{
		menu(wParam, hwnd);

		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
		{
			veterinario* nodo = new veterinario;		
			

			if (IMAGE)			
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT4);
				int Totaltext = GetWindowTextLength(hEdit);

				if (Totaltext != 0)
				{
					char text[50];
					GetDlgItemText(hwnd, IDC_EDIT4, text, sizeof(text));
					int Totallet = strlen(text);
					bool h = 1, y = 1;



					if (Totallet >= 5 && Totallet <= 50)
					{
						for (int i = 0; i < Totallet; i++)
						{
							if (isdigit(text[i]))
							{
								y = 0;
							}

							if (ispunct(text[i]))
							{
								h = 0;
							}
						}
						if (y == 1 && h == 1)
						{



							HWND hEdit = GetDlgItem(hwnd, IDC_EDIT5);
							int Totaltext = GetWindowTextLength(hEdit);

							if (Totaltext != 0)
							{
								HWND hEdit = GetDlgItem(hwnd, IDC_EDIT2);
								int Totaltext = GetWindowTextLength(hEdit);

								if (Totaltext != 0)
								{
									HWND hEdit = GetDlgItem(hwnd, IDC_EDIT3);
									int Totaltext = GetWindowTextLength(hEdit);

									if (Totaltext != 0)
									{

										GetDlgItemText(hwnd, IDC_EDIT4, nodo->NOMBREV, sizeof(nodo->NOMBREV));
										GetDlgItemText(hwnd, IDC_EDIT5, nodo->CEDULA, sizeof(nodo->CEDULA));
										GetDlgItemText(hwnd, IDC_EDIT2, nodo->NomU, sizeof(nodo->NomU));
										GetDlgItemText(hwnd, IDC_EDIT3, nodo->contrasena, sizeof(nodo->contrasena));
										strcpy_s(nodo->FOTO, zFile);

										REGAUSU(nodo);
										MessageBox(hwnd, "veterinarios registrado", "AVISO", MB_OK | MB_ICONINFORMATION);

										SendMessage(GetDlgItem(hwnd, IDC_EDIT4), WM_SETTEXT, 0, (LPARAM)L"");
										SendMessage(GetDlgItem(hwnd, IDC_EDIT5), WM_SETTEXT, 0, (LPARAM)L"");
										SendMessage(GetDlgItem(hwnd, IDC_EDIT2), WM_SETTEXT, 0, (LPARAM)L"");
										SendMessage(GetDlgItem(hwnd, IDC_EDIT3), WM_SETTEXT, 0, (LPARAM)L"");


										SendDlgItemMessage(hwnd, IDC_I, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)L"");
									}
									else
									{
										MessageBox(hwnd, "Ingrese una Contraseña", "AVISO", MB_OK | MB_ICONINFORMATION);
									}
								}
								else
								{
									MessageBox(hwnd, "Ingrese un nombre de usuario", "AVISO", MB_OK | MB_ICONINFORMATION);
								}
							}
							else
							{
								MessageBox(hwnd, "Ingrese una cedula", "AVISO", MB_OK | MB_ICONINFORMATION);
							}

							

						}
						else
						{
							MessageBox(hwnd, "Solo se permiten letras, el nombre no debe llevar numeros ni caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
						}

					}
					else
					{
						MessageBox(hwnd, "El nombre debe tener minimo 5 letras", "ADVERTENCIA", MB_OK | MB_ICONERROR);
					}


				}
				else
				{
					MessageBox(hwnd, "Ingrese un nombre de veterinarios", "AVISO", MB_OK | MB_ICONINFORMATION);
				}
							
			}
			else
			{
				MessageBox(hwnd, "Propocione una imagen", "AVISO", MB_OK | MB_ICONINFORMATION);
			}


		}break;

		case IDC_BUTTON2:
		{
			if (BOT == nullptr)
			{
				EndDialog(hwnd, 0);
				DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG3), NULL, INICIOSesion);
			}

			else
			{
				EndDialog(hwnd, 0);
				DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG1), NULL, PRINCIPAL);
			}




		}break;

		case IDC_BUTTON3:
		{

			
			OPENFILENAME ofn;

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp; //1

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE); //2

				SendDlgItemMessage(hwnd, IDC_I, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

				IMAGE = 1;
			}
			else {
				MessageBox(hwnd, "Seleccione una imagen de sus archivos", "AVISO", MB_OK | MB_ICONINFORMATION);
			}


		}break;




		}
	}break;
	}



	return 0;
}
void REGAUSU(veterinario* nuevo)
{
	if (inicioV == nullptr) {
		inicioV = new veterinario;
		AuxV = inicioV;

		strcpy_s(AuxV->NomU, nuevo->NomU);
		strcpy_s(AuxV->contrasena, nuevo->contrasena);
		strcpy_s(AuxV->CEDULA, nuevo->CEDULA);
		strcpy_s(AuxV->NOMBREV, nuevo->NOMBREV);		
		strcpy_s(AuxV->FOTO, nuevo->FOTO);
		AuxV->sigV = nullptr;
		AuxV->anteV = nullptr;

	}
	else {
		AuxV = inicioV;

		while (AuxV->sigV != nullptr)
		{
			AuxV = AuxV->sigV;
		}

		AuxV->sigV = new veterinario;

		AuxV = AuxV->sigV;

		while (inicioV->sigV != AuxV)
		{
			inicioV = inicioV->sigV;
		}

		strcpy_s(AuxV->NomU, nuevo->NomU);
		strcpy_s(AuxV->contrasena, nuevo->contrasena);
		strcpy_s(AuxV->CEDULA, nuevo->CEDULA);
		strcpy_s(AuxV->NOMBREV, nuevo->NOMBREV);
		strcpy_s(AuxV->FOTO, nuevo->FOTO);
		AuxV->sigV = nullptr;
		AuxV->anteV = inicioV;

		while (inicioV->anteV != nullptr)
		{
			inicioV = inicioV->anteV;
		}

	}



}
void escribirMedico()
{
	AuxV = inicioV;
	ofstream escribir;
	escribir.open("CAMBIAR", ios::out | ios::binary | ios::trunc);

	if (escribir.is_open()) {
		while (AuxV != nullptr)
		{
			escribir.write((char*)AuxV, sizeof(veterinario));
			AuxV = AuxV->sigV;
		}

		escribir.close();
	}

}
void leerMedico()
{
	ifstream leer;
	leer.open("CAMBIAR", ios::in | ios::binary);


	AuxV = inicioV;
	if (leer.is_open()) {

		veterinario* citLeido = new veterinario;

		while (!leer.read((char*)citLeido, sizeof(veterinario)).eof()) {

			while (AuxV != nullptr && AuxV->sigV != nullptr)
			{
				AuxV = AuxV->sigV;
			}
			if (AuxV == nullptr) {
				inicioV = citLeido;
				inicioV->sigV = nullptr;
				inicioV->anteV = nullptr;
				AuxV = inicioV;
			}
			else {


				AuxV->sigV = citLeido;
				citLeido->anteV = AuxV;
				AuxV = AuxV->sigV;
				AuxV->sigV = nullptr;



			}

			citLeido = new veterinario;
		}
		leer.close();
	}
}
BOOL CALLBACK INICIOSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc)
		{
		case IDYES:
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}break;
		}
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {

		case IDC_BUTTON1:
		{
			EndDialog(hwnd, 0);
			DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG2), NULL, veterinarios);


		}break;

		case IDC_BUTTON2: {

			char empz[50] = { 0 };
			char term[10] = { 0 };


			GetDlgItemText(hwnd, IDC_EDIT1, empz, 50);
			GetDlgItemText(hwnd, IDC_EDIT2, term, 10);

			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
			int Totaltext = GetWindowTextLength(hEdit);

			if(Totaltext!=0)
			{
				if (inicioV == nullptr) {
					MessageBox(hwnd, "No hay medicos registrados. Registre alguno", "AVISO", MB_OK | MB_ICONINFORMATION);
				}
				else {
					AuxV = inicioV;
					while (AuxV != nullptr && strcmp(empz, AuxV->NomU) != 0) {
						AuxV = AuxV->sigV;
					}

					if (AuxV == nullptr) {
						MessageBox(hwnd, "usuario no encontrado o nombre equivocado.", "AVISO", MB_OK | MB_ICONINFORMATION);
					}
					else {

						if (strcmp(term, AuxV->contrasena) == 0)
						{
							BOT = AuxV;
							EndDialog(hwnd, 0);
							DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG1), NULL, PRINCIPAL);
						}
						else
						{
							MessageBox(hwnd, "La contraseña no coincide.", "AVISO", MB_OK | MB_ICONINFORMATION);
						}

					}
				}
			}
			else
			{
				MessageBox(hwnd, "Por favor rellene bien las casillas", "AVISO", MB_OK | MB_ICONINFORMATION);
			}

		}break;
		}
	}break;
	}


	return 0;
}
BOOL CALLBACK PRINCIPAL(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
	{
		

		SetDlgItemText(hwnd, IDC_EDIT1, BOT->NOMBREV);
		SetDlgItemText(hwnd, IDC_EDIT2, BOT->CEDULA);

		SYSTEMTIME fechahoy;
		ZeroMemory(&fechahoy, sizeof(fechahoy));

		GetLocalTime(&fechahoy);


		char fechaCompleta[11] = { 0 };
		char numCar[5] = { 0 };

		_itoa_s(fechahoy.wDay, numCar, 10);
		strcat_s(fechaCompleta, numCar);

		strcat_s(fechaCompleta, "/");

		_itoa_s(fechahoy.wMonth, numCar, 10);
		strcat_s(fechaCompleta, numCar);

		strcat_s(fechaCompleta, "/");

		_itoa_s(fechahoy.wYear, numCar, 10);
		strcat_s(fechaCompleta, numCar);


		SetDlgItemText(hwnd, IDC_EDIT3, fechaCompleta);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, BOT->FOTO, IMAGE_BITMAP, 120, 120, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, IDC_Imajen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

	}break;

	case WM_CLOSE:
	{
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc)
		{
		case IDYES:
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}break;
		}
	}break;


	case WM_COMMAND: {

		menu(wParam, hwnd);

	}break;

	}


	return 0;
}
BOOL CALLBACK CITAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc)
		{
		case IDYES:
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}break;
		}
	}break;

	case WM_INITDIALOG:
	{
		SetDlgItemText(hwnd, IDC_EDIT1, BOT->NOMBREV);
		HBITMAP bmp;

		bmp = (HBITMAP)LoadImage(NULL, BOT->FOTO, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);

		SendDlgItemMessage(hwnd, IDC_FOTOIS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, NULL, (LPARAM)"Perro");
		SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, NULL, (LPARAM)"Gato");
		SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, NULL, (LPARAM)"Ave");
		SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, NULL, (LPARAM)"Ruedor");
		SendDlgItemMessage(hwnd, IDC_COMBO2, CB_INSERTSTRING, NULL, (LPARAM)"Otro");

	}break;

	case WM_COMMAND:
	{
		menu(wParam, hwnd);

		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			cita* nodo = new cita;
			bool n = 1;
			bool c = 1, v = 1, b = 1, que = 1;			
			int r;
			char m[50], me[30], sem[30], d[20], h[25], j[50], RangTEL[12], RangPre[20];

			if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED)
			{
				nodo->Estado = 1; 
			}
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED)
			{
				nodo->Estado = 2; 			
			}
			else if (IsDlgButtonChecked(hwnd, IDC_RADIO3) == BST_CHECKED)
			{
				nodo->Estado = 3; 
			}
			else
			{
				MessageBox(hwnd, "Seleccione Un estado de la cita; Pendiente, Efectuada, etc.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
			}

			if (nodo->Estado != 0)
			{
				AuxC = inicioC;

				if (inicioC == nullptr)
				{
					n = 0;

					HWND hEdit = GetDlgItem(hwnd, IDC_EDIT6);
					int Totaltext = GetWindowTextLength(hEdit);

					if (Totaltext != 0)
					{
						HWND hEdit = GetDlgItem(hwnd, IDC_EDIT7);
						int Totaltext = GetWindowTextLength(hEdit);

						if (Totaltext != 0)
						{


							char text[50];
							GetDlgItemText(hwnd, IDC_EDIT2, text, sizeof(text));
							int Totallet = strlen(text);
							bool h = 1, y = 1;

							if (Totallet >= 0 && Totallet <= 50)
							{
								for (int i = 0; i < Totallet; i++)
								{
									if (isdigit(text[i]))
									{
										y = 0;
									}

									if (ispunct(text[i]))
									{
										h = 0;
									}
								}
								if (y == 1 && h == 1)
								{

									bool segu = 0, terc = 0;
									GetDlgItemText(hwnd, IDC_EDIT3, RangTEL, sizeof(RangTEL));

									int totalcontra = strlen(RangTEL);

									if (totalcontra >= 8 && totalcontra == 10)
									{
										for (int i = 0; i < totalcontra; i++)
										{
											if (isalpha(RangTEL[i]))
											{
												segu = 1;

											}
											if (ispunct(RangTEL[i]))
											{
												terc = 1;

											}


										}
										if (segu == 1)
										{
											MessageBox(hwnd, "No se permiten letras en el telefono.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
										}
										if (terc == 1)
										{
											MessageBox(hwnd, "No se permten caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
										}
										if (segu == 0 && terc == 0)
										{
											char text[50];
											GetDlgItemText(hwnd, IDC_EDIT5, text, sizeof(text));
											int Totallet = strlen(text);
											h = 1, y = 1;

											if (Totallet >= 0 && Totallet <= 50)
											{
												for (int i = 0; i < Totallet; i++)
												{
													if (isdigit(text[i]))
													{
														y = 0;
													}

													if (ispunct(text[i]))
													{
														h = 0;
													}
												}
												if (y == 1 && h == 1)
												{
													bool segu = 0, terc = 0;
													GetDlgItemText(hwnd, IDC_EDIT8, RangPre, sizeof(RangPre));

													int totalcontra = strlen(RangPre);
													for (int i = 0; i < totalcontra; i++)
													{
														if (isalpha(RangPre[i]))
														{
															segu = 1;

														}
														if (ispunct(RangPre[i]))
														{
															if (RangPre[i] != '.')
															{
																terc = 1;
															}
														}
													}
													if (segu == 1)
													{
														MessageBox(hwnd, "No se permiten letras en el telefono.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
													}
													if (terc == 1)
													{
														MessageBox(hwnd, "No se permten caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
													}
													if (segu == 0 && terc == 0)
													{
														GetDlgItemText(hwnd, IDC_EDIT2, nodo->NomC, sizeof(nodo->NomC));
														GetDlgItemText(hwnd, IDC_EDIT3, nodo->TEL, sizeof(nodo->TEL));
														GetDlgItemText(hwnd, IDC_EDIT5, nodo->NomM, sizeof(nodo->NomM));
														GetDlgItemText(hwnd, IDC_COMBO2, nodo->Especie, sizeof(nodo->Especie));
														GetDlgItemText(hwnd, IDC_EDIT6, nodo->Fecha, sizeof(nodo->Fecha));
														GetDlgItemText(hwnd, IDC_EDIT7, nodo->Hora, sizeof(nodo->Hora));
														GetDlgItemText(hwnd, IDC_EDIT9, nodo->Motivo, sizeof(nodo->Motivo));
														GetDlgItemText(hwnd, IDC_EDIT8, nodo->Precio, sizeof(nodo->Precio));
														GetDlgItemText(hwnd, IDC_EDIT10, nodo->MES, sizeof(nodo->MES));

														registrarCit(nodo);
														MessageBox(hwnd, "Cita registrada", "AVISO", MB_OK | MB_ICONINFORMATION);

													}



												}
												else
												{
													MessageBox(hwnd, "Solo se permiten letras, el nombre no debe llevar numeros ni caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
												}

											}

										}

									}
									else
									{
										MessageBox(hwnd, "El rango debe ser mayor de 8 y menor de 10.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
									}






								}
								else
								{
									MessageBox(hwnd, "Solo se permiten letras, el nombre no debe llevar numeros ni caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
								}
							}
							else
							{
								MessageBox(hwnd, "Relle las casillas antes de dar aceptar.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
							}		
							
						}
						else
						{
							MessageBox(hwnd, "Rellene las casillas faltantes", "AVISO", MB_OK | MB_ICONINFORMATION);

						}
					}
					else
					{
						MessageBox(hwnd, "Rellene las casillas faltantes", "AVISO", MB_OK | MB_ICONINFORMATION);

					}
				}
				if (n == 1)
				{
					while (AuxC != nullptr)
					{
						GetDlgItemText(hwnd, IDC_EDIT6, d, sizeof(d));
						r = strcmp(AuxC->Fecha, d);

						if (r == 0)
						{

							b = 0;

						}

						GetDlgItemText(hwnd, IDC_EDIT7, h, sizeof(h));
						r = strcmp(AuxC->Hora, h);

						if (r == 0)
						{

							que = 0;

						}

						AuxC = AuxC->sigC;

					}

					if (b == 1 || que == 1)
					{
						
						char text[50];
						GetDlgItemText(hwnd, IDC_EDIT2, text, sizeof(text));
						int Totallet = strlen(text);
						bool h = 1, y = 1;

						if (Totallet >= 0 && Totallet <= 50)
						{
							for (int i = 0; i < Totallet; i++)
							{
								if (isdigit(text[i]))
								{
									y = 0;
								}

								if (ispunct(text[i]))
								{
									h = 0;
								}
							}
							if (y == 1 && h == 1)
							{
								
								bool segu = 0, terc = 0;
								GetDlgItemText(hwnd, IDC_EDIT3, RangTEL, sizeof(RangTEL));

								int totalcontra = strlen(RangTEL);

								if (totalcontra >= 8 && totalcontra == 10)
								{
									for (int i = 0; i < totalcontra; i++)
									{
										if (isalpha(RangTEL[i]))
										{
											segu = 1;

										}
										if (ispunct(RangTEL[i]))
										{
											terc = 1;

										}


									}
									if (segu == 1)
									{
										MessageBox(hwnd, "No se permiten letras en el telefono.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
									}
									if (terc == 1)
									{
										MessageBox(hwnd, "No se permten caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
									}
									if (segu == 0 && terc == 0)
									{
										char text[50];
										GetDlgItemText(hwnd, IDC_EDIT5, text, sizeof(text));
										int Totallet = strlen(text);
										h = 1, y = 1;

										if (Totallet >= 0 && Totallet <= 50)
										{
											for (int i = 0; i < Totallet; i++)
											{
												if (isdigit(text[i]))
												{
													y = 0;
												}

												if (ispunct(text[i]))
												{
													h = 0;
												}
											}
											if (y == 1 && h == 1)
											{
												bool segu = 0, terc = 0;
												GetDlgItemText(hwnd, IDC_EDIT8, RangPre, sizeof(RangPre));

												int totalcontra = strlen(RangPre);
												for (int i = 0; i < totalcontra; i++)
												{
													if (isalpha(RangPre[i]))
													{
														segu = 1;

													}
													if (ispunct(RangPre[i]))
													{
														if(RangPre[i] != '.')
														{
															terc = 1;
														}

													}
												}
												if (segu == 1)
												{
													MessageBox(hwnd, "No se permiten letras en el telefono.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
												}
												if (terc == 1)
												{
													MessageBox(hwnd, "No se permten caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
												}
												if (segu == 0 && terc == 0)
												{
													GetDlgItemText(hwnd, IDC_EDIT2, nodo->NomC, sizeof(nodo->NomC));
													GetDlgItemText(hwnd, IDC_EDIT3, nodo->TEL, sizeof(nodo->TEL));
													GetDlgItemText(hwnd, IDC_EDIT5, nodo->NomM, sizeof(nodo->NomM));
													GetDlgItemText(hwnd, IDC_COMBO2, nodo->Especie, sizeof(nodo->Especie));
													GetDlgItemText(hwnd, IDC_EDIT6, nodo->Fecha, sizeof(nodo->Fecha));
													GetDlgItemText(hwnd, IDC_EDIT7, nodo->Hora, sizeof(nodo->Hora));
													GetDlgItemText(hwnd, IDC_EDIT9, nodo->Motivo, sizeof(nodo->Motivo));
													GetDlgItemText(hwnd, IDC_EDIT8, nodo->Precio, sizeof(nodo->Precio));
													GetDlgItemText(hwnd, IDC_EDIT10, nodo->MES, sizeof(nodo->MES));

													registrarCit(nodo);
													MessageBox(hwnd, "Cita registrada", "AVISO", MB_OK | MB_ICONINFORMATION);

												}



											}
											else
											{
												MessageBox(hwnd, "Solo se permiten letras, el nombre no debe llevar numeros ni caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
											}

										}
										
									}

								}
								else
								{
									MessageBox(hwnd, "El rango debe ser mayor de 8 y menor de 10.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
								}


								



							}
							else
							{
								MessageBox(hwnd, "Solo se permiten letras, el nombre no debe llevar numeros ni caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
							}
						}
						else
						{
							MessageBox(hwnd, "Relle las casillas antes de dar aceptar.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
						}
					}
					else
					{
						MessageBox(hwnd, "Las fechas ya estan ocupadas", "AVISO", MB_OK | MB_ICONINFORMATION);

					}				
				}				
			}	
		
		}break;

		case IDCANCEL:
		{			
				EndDialog(hwnd, 0);
				DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG1), NULL, PRINCIPAL);

		}break;

		}
	}break;
	}
	return 0;
}
void registrarCit(cita* nuevo)
{
	if (inicioC == nullptr) {
		inicioC = new cita;
		AuxC = inicioC;

		strcpy_s(AuxC->NomC, nuevo->NomC);
		strcpy_s(AuxC->TEL, nuevo->TEL);
		strcpy_s(AuxC->Especie, nuevo->Especie);
		strcpy_s(AuxC->NomM, nuevo->NomM);
		strcpy_s(AuxC->Fecha, nuevo->Fecha);
		strcpy_s(AuxC->Hora, nuevo->Hora);
		strcpy_s(AuxC->Motivo, nuevo->Motivo);
		AuxC->Estado = nuevo->Estado;
		strcpy_s(AuxC->Precio, nuevo->Precio);
		strcpy_s(AuxC->MES, nuevo->MES);
		AuxC->sigC = nullptr;
		AuxC->anteC= nullptr;

	}
	else {
		AuxC = inicioC;

		while (AuxC->sigC != nullptr)
		{
			AuxC = AuxC->sigC;
		}

		AuxC->sigC = new cita;

		AuxC = AuxC->sigC;

		while (inicioC->sigC != AuxC)
		{
			inicioC = inicioC->sigC;
		}

		strcpy_s(AuxC->NomC, nuevo->NomC);
		strcpy_s(AuxC->TEL, nuevo->TEL);
		strcpy_s(AuxC->Especie, nuevo->Especie);
		strcpy_s(AuxC->NomM, nuevo->NomM);
		strcpy_s(AuxC->Fecha, nuevo->Fecha);
		strcpy_s(AuxC->Hora, nuevo->Hora);
		strcpy_s(AuxC->Motivo, nuevo->Motivo);
		AuxC->Estado = nuevo->Estado;
		strcpy_s(AuxC->Precio, nuevo->Precio);
		strcpy_s(AuxC->MES, nuevo->MES);
		AuxC->sigC = nullptr;
		AuxC->anteC= inicioC;

		while (inicioC->anteC!= nullptr)
		{
			inicioC = inicioC->anteC;
		}

	}


}
void escribirCitas()
{
	AuxC = inicioC;
	ofstream escribir;
	escribir.open("Escribir ruta de donde se guardo la carpeta del proyecto", ios::out | ios::binary | ios::trunc);

	if (escribir.is_open()) {
		while (AuxC != nullptr)
		{
			escribir.write((char*)AuxC, sizeof(cita));
			AuxC = AuxC->sigC;
		}

		escribir.close();
	}
}
void leerCitas()
{
	ifstream leer;
	leer.open("Escribir ruta de donde se guardo la carpeta del proyecto", ios::in | ios::binary);


	AuxC = inicioC;
	if (leer.is_open()) {

		cita* citLeido = new cita;

		while (!leer.read((char*)citLeido, sizeof(cita)).eof()) {

			while (AuxC != nullptr && AuxC->sigC != nullptr)
			{
				AuxC = AuxC->sigC;
			}
			if (AuxC == nullptr) {
				inicioC = citLeido;
				inicioC->sigC = nullptr;
				inicioC->anteC = nullptr;
				AuxC = inicioC;
			}
			else {


				AuxC->sigC = citLeido;
				citLeido->anteC = AuxC;
				AuxC = AuxC->sigC;
				AuxC->sigC = nullptr;



			}

			citLeido = new cita;
		}
		leer.close();
	}
}
BOOL CALLBACK AGENDA(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc)
		{
		case IDYES:
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}break;
		}
	}break;

	case WM_INITDIALOG:
	{
		SetDlgItemText(hwnd, IDC_EDIT1, BOT->NOMBREV);

		HBITMAP bmp;

		bmp = (HBITMAP)LoadImage(NULL, BOT->FOTO, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);

		SendDlgItemMessage(hwnd, IDC_FOTOIS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		SendDlgItemMessage(hwnd, IDC_COMBO5, CB_INSERTSTRING, NULL, (LPARAM)"Perro");
		SendDlgItemMessage(hwnd, IDC_COMBO5, CB_INSERTSTRING, NULL, (LPARAM)"Gato");
		SendDlgItemMessage(hwnd, IDC_COMBO5, CB_INSERTSTRING, NULL, (LPARAM)"Ave");
		SendDlgItemMessage(hwnd, IDC_COMBO5, CB_INSERTSTRING, NULL, (LPARAM)"Ruedor");
		SendDlgItemMessage(hwnd, IDC_COMBO5, CB_INSERTSTRING, NULL, (LPARAM)"Otro");

		AuxC = inicioC;
		while (AuxC != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)AuxC->NomC);
			AuxC = AuxC->sigC;
		}
		AuxC = inicioC;

	}break;

	case WM_COMMAND: 
	{
		menu(wParam, hwnd);

		switch (LOWORD(wParam)) 
		{
		case IDC_LIST1:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{

				

				int indice = 0;
				char textoList[30] = { 0 };
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoList);

				AuxC = inicioC;
				while (AuxC->sigC != nullptr && strcmp(textoList, AuxC->NomC) != 0) {
					AuxC = AuxC->sigC;
				}

				SetDlgItemText(hwnd, IDC_EDIT2, AuxC->NomC);
				SetDlgItemText(hwnd, IDC_EDIT3, AuxC->TEL);
				SetDlgItemText(hwnd, IDC_EDIT5, AuxC->NomM);
				SetDlgItemText(hwnd, IDC_EDIT6, AuxC->Fecha);
				SetDlgItemText(hwnd, IDC_EDIT4, AuxC->MES);
				SetDlgItemText(hwnd, IDC_EDIT7, AuxC->Hora);
				SetDlgItemText(hwnd, IDC_EDIT9, AuxC->Motivo);
				SetDlgItemText(hwnd, IDC_EDIT8, AuxC->Precio);

				
				

				if (AuxC->Estado == 1) {
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO3, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}
				else if (AuxC->Estado == 2) {
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO3, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}			
				else if (AuxC->Estado == 3) {
					SendDlgItemMessage(hwnd, IDC_RADIO3, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
					SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				}

				SendMessage(GetDlgItem(hwnd, IDC_COMBO5), CB_SELECTSTRING, 0, (LPARAM)AuxC->Especie);
			

			}break;

			

			}
		}break;

		case IDCANCEL:
		{
			EndDialog(hwnd, 0);
			DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG1), NULL, PRINCIPAL);

		}break;

		case IDC_BUTTON1:
		{
			int indice = 0;
			char textoList[30] = { 0 };
			indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoList);

			ELIMINAR(textoList, hwnd);

		}break;

		case IDC_BUTTON2:
		{
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT5), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT4), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT6), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT7), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT8), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT9), true);

			EnableWindow(GetDlgItem(hwnd, IDC_RADIO1), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO2), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RADIO3), true);

			EnableWindow(GetDlgItem(hwnd, IDC_COMBO5), true);

		
			ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_SHOW);

		}break;

		case IDC_BUTTON3:
		{
			int indice = 0;
			char textoList[30] = { 0 };
			indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoList);

			AuxC = inicioC;
			while (AuxC->sigC != nullptr && strcmp(textoList, AuxC->NomC) != 0) {
				AuxC = AuxC->sigC;
			}

			cita* nodo = new cita;
			bool n = 1;
			bool c = 1, v = 1, b = 1, que = 1;
			int r;
			char m[50], me[30], sem[30], d[20], h[25], j[50], RangTEL[12], RangPre[20];

			SYSTEMTIME fechahoy;
			ZeroMemory(&fechahoy, sizeof(fechahoy));

			GetLocalTime(&fechahoy);

			char fechaCompleta[11] = { 0 };
			char numCar[5] = { 0 };

			_itoa_s(fechahoy.wDay, numCar, 10);
			strcat_s(fechaCompleta, numCar);

			char fechaCompleta2[11] = { 0 };
			char numCar2[5] = { 0 };

			_itoa_s(fechahoy.wMonth, numCar2, 10);
			strcat_s(fechaCompleta2, numCar2);

			if(strcmp(fechaCompleta, AuxC->Fecha) != 0 && strcmp(fechaCompleta2, AuxC->MES) != 0)
			{
				AuxC = inicioC;

				while (AuxC != nullptr)
				{
					GetDlgItemText(hwnd, IDC_EDIT6, d, sizeof(d));
					r = strcmp(AuxC->Fecha, d);

					if (r == 0)
					{

						b = 0;

					}

					GetDlgItemText(hwnd, IDC_EDIT7, h, sizeof(h));
					r = strcmp(AuxC->Hora, h);

					if (r == 0)
					{

						que = 0;

					}

					AuxC = AuxC->sigC;

				}

				if (b == 1 || que == 1)
				{

					char text[50];
					GetDlgItemText(hwnd, IDC_EDIT2, text, sizeof(text));
					int Totallet = strlen(text);
					bool h = 1, y = 1;

					if (Totallet >= 0 && Totallet <= 50)
					{
						for (int i = 0; i < Totallet; i++)
						{
							if (isdigit(text[i]))
							{
								y = 0;
							}

							if (ispunct(text[i]))
							{
								h = 0;
							}
						}
						if (y == 1 && h == 1)
						{

							bool segu = 0, terc = 0;
							GetDlgItemText(hwnd, IDC_EDIT3, RangTEL, sizeof(RangTEL));

							int totalcontra = strlen(RangTEL);

							if (totalcontra >= 8 && totalcontra == 10)
							{
								for (int i = 0; i < totalcontra; i++)
								{
									if (isalpha(RangTEL[i]))
									{
										segu = 1;

									}
									if (ispunct(RangTEL[i]))
									{
										terc = 1;

									}


								}
								if (segu == 1)
								{
									MessageBox(hwnd, "No se permiten letras en el telefono.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
								}
								if (terc == 1)
								{
									MessageBox(hwnd, "No se permten caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
								}
								if (segu == 0 && terc == 0)
								{
									char text[50];
									GetDlgItemText(hwnd, IDC_EDIT5, text, sizeof(text));
									int Totallet = strlen(text);
									h = 1, y = 1;

									if (Totallet >= 0 && Totallet <= 50)
									{
										for (int i = 0; i < Totallet; i++)
										{
											if (isdigit(text[i]))
											{
												y = 0;
											}

											if (ispunct(text[i]))
											{
												h = 0;
											}
										}
										if (y == 1 && h == 1)
										{
											bool segu = 0, terc = 0;
											GetDlgItemText(hwnd, IDC_EDIT8, RangPre, sizeof(RangPre));

											int totalcontra = strlen(RangPre);
											for (int i = 0; i < totalcontra; i++)
											{
												if (isalpha(RangPre[i]))
												{
													segu = 1;

												}
												if (ispunct(RangPre[i]))
												{
													if (RangPre[i] != '.')
													{
														terc = 1;
													}

												}
											}
											if (segu == 1)
											{
												MessageBox(hwnd, "No se permiten letras en el telefono.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
											}
											if (terc == 1)
											{
												MessageBox(hwnd, "No se permten caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
											}
											if (segu == 0 && terc == 0)
											{
												if (IsDlgButtonChecked(hwnd, IDC_RADIO1) == BST_CHECKED)
												{
													nodo->Estado = 1;
												}
												else if (IsDlgButtonChecked(hwnd, IDC_RADIO2) == BST_CHECKED)
												{
													nodo->Estado = 2;
												}
												else if (IsDlgButtonChecked(hwnd, IDC_RADIO3) == BST_CHECKED)
												{
													nodo->Estado = 3;
												}

												GetDlgItemText(hwnd, IDC_EDIT2, nodo->NomC, sizeof(nodo->NomC));
												GetDlgItemText(hwnd, IDC_EDIT3, nodo->TEL, sizeof(nodo->TEL));
												GetDlgItemText(hwnd, IDC_EDIT5, nodo->NomM, sizeof(nodo->NomM));
												GetDlgItemText(hwnd, IDC_COMBO5, nodo->Especie, sizeof(nodo->Especie));
												GetDlgItemText(hwnd, IDC_EDIT6, nodo->Fecha, sizeof(nodo->Fecha));
												GetDlgItemText(hwnd, IDC_EDIT4, nodo->MES, sizeof(nodo->MES));
												GetDlgItemText(hwnd, IDC_EDIT7, nodo->Hora, sizeof(nodo->Hora));
												GetDlgItemText(hwnd, IDC_EDIT9, nodo->Motivo, sizeof(nodo->Motivo));
												GetDlgItemText(hwnd, IDC_EDIT8, nodo->Precio, sizeof(nodo->Precio));


												MODIFICAR(textoList, hwnd, nodo);
												

											}



										}
										else
										{
											MessageBox(hwnd, "Solo se permiten letras, el nombre no debe llevar numeros ni caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
										}

									}

								}

							}
							else
							{
								MessageBox(hwnd, "El rango debe ser mayor de 8 y menor de 10.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
							}
						}
						else
						{
							MessageBox(hwnd, "Solo se permiten letras, el nombre no debe llevar numeros ni caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
						}
					}
					else
					{
						MessageBox(hwnd, "Relle las casillas antes de dar aceptar.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
					}
				}
				else
				{
					MessageBox(hwnd, "Las fechas ya estan ocupadas", "AVISO", MB_OK | MB_ICONINFORMATION);

				}
			}

		}break;
		
		case IDC_BUTTON4:
		{
			AuxC = inicioC;
			bool F = 1;

			ofstream reporte;
			reporte.open("C:\Users\PCMR\Downloads\AGENDA(1)\AGENDA(1)\AGENDA", ios::out | ios::trunc);

			if (reporte.is_open()) {
				while (AuxC != nullptr)
				{
					if(F)
					{
						reporte << "Nombre pas." << "," << "Tel" << "," << "Especie" << "," << "Nombre MAS" << "," << "Dia" << "," << "Mes" << "," << "Hora" << "," << "Estado" << "," << "Motivo" << "," << "Precio" << endl;
						F = 0;
					}

					/*reporte.write((char*)auxcli, sizeof(cliente));*/
					char tipo[30] = { 0 };
					reporte << AuxC->NomC << "," << AuxC->TEL << "," << AuxC->Especie << "," << AuxC->NomM << "," << AuxC->Fecha << "," << AuxC->MES << "," << AuxC->Hora << "," << AuxC->Estado << "," << AuxC->Motivo << "," << AuxC->Precio << endl;
					AuxC = AuxC->sigC;
				}

				reporte.close();
			}

			MessageBox(hwnd, "Archivo de reporte creado.", "AVISO", MB_OK | MB_ICONINFORMATION);

		}break;

		case IDC_BUTTON5:
		{
			char fe[6], me[6];

			AuxC = inicioC;
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);

			GetDlgItemText(hwnd, IDC_EDIT10, fe, sizeof(fe));

			while (AuxC != nullptr)
			{
				if (strcmp(AuxC->Fecha, fe) == 0)
				{
					GetDlgItemText(hwnd, IDC_EDIT11, me, sizeof(me));

					if (strcmp(AuxC->MES, me) == 0)
					{
						SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)AuxC->NomC);

					}
				}
				AuxC = AuxC->sigC;
			}		
			
		}break;

		}
	}break;
	}

	return 0;
}
void ELIMINAR(char Eliminar[50], HWND hwnd)
{
	AuxC = inicioC;

	if (AuxC == nullptr)
	{
		MessageBox(hwnd, "No hay citas registradas", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else 
	{		
		while (AuxC != nullptr && strcmp(AuxC->NomC, Eliminar) != 0) 
		{ 			
			AuxC = AuxC->sigC;
		}

		if (AuxC == nullptr) 
		{

		}
		else if (AuxC == inicioC)
		{	

			inicioC = inicioC->sigC; 
			

			delete AuxC;		

			AuxC = inicioC;	

			if (AuxC != nullptr)
			{
				inicioC->anteC = nullptr;
			}
		

			MessageBox(hwnd, "Cita eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);

			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
			AuxC = inicioC;
			while (AuxC != nullptr) 
			{
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)AuxC->NomC);
				AuxC = AuxC->sigC;
			}
		}
		else 
		{
			if(AuxC->sigC == nullptr)
			{
				AuxC->anteC->sigC = AuxC->sigC;
			

				delete AuxC;

				AuxC = inicioC;

				MessageBox(hwnd, "Cita eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);
			}
			else
			{
				AuxC->anteC->sigC = AuxC->sigC;
				AuxC->sigC->anteC = AuxC->anteC;

				delete AuxC;

				AuxC = inicioC;

				MessageBox(hwnd, "Cita eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);

			}

		}

		SetDlgItemText(hwnd, IDC_EDIT2, "");
		SetDlgItemText(hwnd, IDC_EDIT3, "");
		SetDlgItemText(hwnd, IDC_EDIT5, "");
		SetDlgItemText(hwnd, IDC_EDIT6, "");
		SetDlgItemText(hwnd, IDC_EDIT4, "");
		SetDlgItemText(hwnd, IDC_EDIT7, "");
		SetDlgItemText(hwnd, IDC_EDIT9, "");
		SetDlgItemText(hwnd, IDC_EDIT8, "");
		SendDlgItemMessage(hwnd, IDC_RADIO1, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
		SendDlgItemMessage(hwnd, IDC_RADIO2, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
		SendDlgItemMessage(hwnd, IDC_RADIO3, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
		SetDlgItemText(hwnd, IDC_COMBO5, "");
	


	}

}
void MODIFICAR(char Mod[], HWND hwnd, cita* nuevo)
{
	AuxC = inicioC;

	while (AuxC != nullptr && strcmp(Mod, AuxC->NomC) != 0) { 
		AuxC = AuxC->sigC;
	}

	if (AuxC == nullptr) 
	{	
	}
	else {											

		strcpy_s(AuxC->NomC, nuevo->NomC);
		strcpy_s(AuxC->TEL, nuevo->TEL);
		strcpy_s(AuxC->Especie, nuevo->Especie);
		strcpy_s(AuxC->NomM, nuevo->NomM);
		strcpy_s(AuxC->Fecha, nuevo->Fecha);
		strcpy_s(AuxC->Hora, nuevo->Hora);
		strcpy_s(AuxC->Motivo, nuevo->Motivo);
		AuxC->Estado = nuevo->Estado;
		strcpy_s(AuxC->Precio, nuevo->Precio);
		strcpy_s(AuxC->MES, nuevo->MES);

		MessageBox(hwnd, "Usuario modificado", "AVISO", MB_OK | MB_ICONINFORMATION);

		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
		AuxC = inicioC;
		while (AuxC != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)AuxC->NomC);
			AuxC = AuxC->sigC;
		}

		EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), false);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), false);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT5), false);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT6), false);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT4), false);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT7), false);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT9), false);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT8), false);

		EnableWindow(GetDlgItem(hwnd, IDC_RADIO1), false);
		EnableWindow(GetDlgItem(hwnd, IDC_RADIO2), false);
		EnableWindow(GetDlgItem(hwnd, IDC_RADIO3), false);

		EnableWindow(GetDlgItem(hwnd, IDC_COMBO5), false);

		ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_HIDE);
	}
}
BOOL CALLBACK INFOMED(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		switch (opc)
		{
		case IDYES:
		{
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}break;
		}
	}break;

	case WM_INITDIALOG:
	{
		SetDlgItemText(hwnd, IDC_EDIT1, BOT->NOMBREV);

		HBITMAP bmp;

		bmp = (HBITMAP)LoadImage(NULL, BOT->FOTO, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);

		SendDlgItemMessage(hwnd, IDC_FOTOIS, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		AuxV = inicioV;
		while (AuxV != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)AuxV->NOMBREV);
			AuxV = AuxV->sigV;
		}
		AuxV = inicioV;

	}break;

	case WM_COMMAND:
	{
		menu(wParam, hwnd);

		switch (LOWORD(wParam))
		{
		case IDC_LIST1:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				int indice = 0;
				char textoList[30] = { 0 };
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoList);

				AuxV = inicioV;
				while (AuxV->sigV != nullptr && strcmp(textoList, AuxV->NOMBREV) != 0) {
					AuxV = AuxV->sigV;
				}

				SetDlgItemText(hwnd, IDC_EDIT2, AuxV->NOMBREV);
				SetDlgItemText(hwnd, IDC_EDIT3, AuxV->CEDULA);
				SetDlgItemText(hwnd, IDC_EDIT5, AuxV->NomU);
				SetDlgItemText(hwnd, IDC_EDIT7, AuxV->contrasena);
			
				HBITMAP bmp; //1
				bmp = (HBITMAP)LoadImage(NULL, AuxV->FOTO, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE); //2
				SendDlgItemMessage(hwnd, IDC_fotoo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);		


			}break;



			}
		}break;

		case IDCANCEL:
		{
			EndDialog(hwnd, 0);
			DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG1), NULL, PRINCIPAL);

		}break;

		case IDC_BUTTON1:
		{
			int indice = 0;
			char textoList[30] = { 0 };
			indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoList);

			ELIMINARMED(textoList, hwnd);

		}break;

		case IDC_BUTTON2:
		{
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT5), true);
			EnableWindow(GetDlgItem(hwnd, IDC_EDIT7), true);

			ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_SHOW);
			ShowWindow(GetDlgItem(hwnd, IDC_BUTTON4), SW_SHOW);

		}break;

		case IDC_BUTTON3:
		{
			int indice = 0;
			char textoList[30] = { 0 };
			indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETTEXT, indice, (LPARAM)textoList);

			AuxV = inicioV;
			while (AuxV->sigV != nullptr && strcmp(textoList, AuxV->NOMBREV) != 0)
			{
				AuxV = AuxV->sigV;
			}


			veterinario* nodo = new veterinario;

			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT2);
			int Totaltext = GetWindowTextLength(hEdit);

			if (Totaltext != 0)
			{
				char text[50];
				GetDlgItemText(hwnd, IDC_EDIT2, text, sizeof(text));
				int Totallet = strlen(text);
				bool h = 1, y = 1;

				if (Totallet >= 5 && Totallet <= 50)
				{
					for (int i = 0; i < Totallet; i++)
					{
						if (isdigit(text[i]))
						{
							y = 0;
						}

						if (ispunct(text[i]))
						{
							h = 0;
						}
					}
					if (y == 1 && h == 1)
					{
						HWND hEdit = GetDlgItem(hwnd, IDC_EDIT3);
						int Totaltext = GetWindowTextLength(hEdit);

						if (Totaltext != 0)
						{
							HWND hEdit = GetDlgItem(hwnd, IDC_EDIT5);
							int Totaltext = GetWindowTextLength(hEdit);

							if (Totaltext != 0)
							{
								HWND hEdit = GetDlgItem(hwnd, IDC_EDIT7);
								int Totaltext = GetWindowTextLength(hEdit);

								if (Totaltext != 0)
								{

									GetDlgItemText(hwnd, IDC_EDIT2, nodo->NOMBREV, sizeof(nodo->NOMBREV));
									GetDlgItemText(hwnd, IDC_EDIT3, nodo->CEDULA, sizeof(nodo->CEDULA));
									GetDlgItemText(hwnd, IDC_EDIT5, nodo->NomU, sizeof(nodo->NomU));
									GetDlgItemText(hwnd, IDC_EDIT7, nodo->contrasena, sizeof(nodo->contrasena));
									strcpy_s(nodo->FOTO, zFile);

									MODIFICARMED(textoList, hwnd, nodo);

								}
								else
								{
									MessageBox(hwnd, "Ingrese una Contraseña", "AVISO", MB_OK | MB_ICONINFORMATION);
								}
							}
							else
							{
								MessageBox(hwnd, "Ingrese un nombre de usuario", "AVISO", MB_OK | MB_ICONINFORMATION);
							}
						}
						else
						{
							MessageBox(hwnd, "Ingrese una cedula", "AVISO", MB_OK | MB_ICONINFORMATION);
						}



					}
					else
					{
						MessageBox(hwnd, "Solo se permiten letras, el nombre no debe llevar numeros ni caracteres especiales.", "ADVERTENCIA", MB_OK | MB_ICONERROR);
					}
				}
			}
			else
			{
				MessageBox(hwnd, "Ingrese un nombre de veterinarios", "AVISO", MB_OK | MB_ICONINFORMATION);
			}

		}break;

		case IDC_BUTTON4:
		{
			OPENFILENAME ofn;

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp;

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 150, 100, LR_LOADFROMFILE);

				SendDlgItemMessage(hwnd, IDC_fotoo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}

		}break;

		}
		


	}break;
	}


	return 0;
}
void ELIMINARMED(char Eliminar[50], HWND hwnd)
{
	AuxV = inicioV;

	if (AuxV == nullptr)
	{
		MessageBox(hwnd, "No hay citas registradas", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		while (AuxV != nullptr && strcmp(AuxV->NOMBREV, Eliminar) != 0)
		{
			AuxV = AuxV->sigV;
		}

		if (AuxV == nullptr)
		{

		}
		else if (AuxV == inicioV)
		{

			inicioV = inicioV->sigV;
			

			delete AuxV;

			AuxV = inicioV;

			if (AuxV != nullptr)
			{
				inicioV->anteV = nullptr;
			}

			MessageBox(hwnd, "Cita eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);

			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
			AuxV = inicioV;
			while (AuxV != nullptr)
			{
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)AuxV->NOMBREV);
				AuxV = AuxV->sigV;
			}
		}
		else
		{
			if (AuxV->sigV == nullptr)
			{
				AuxV->anteV->sigV = AuxV->sigV;


				delete AuxV;

				AuxV = inicioV;

				MessageBox(hwnd, "Usuario eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);
			}
			else
			{
				AuxV->anteV->sigV = AuxV->sigV;
				AuxV->sigV->anteV = AuxV->anteV;

				delete AuxV;

				AuxV = inicioV;

				MessageBox(hwnd, "Usuario eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_LIST1, LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_DELETESTRING, (WPARAM)indice, 0);

			}

		}


		SetDlgItemText(hwnd, IDC_EDIT2, "");
		SetDlgItemText(hwnd, IDC_EDIT3, "");
		SetDlgItemText(hwnd, IDC_EDIT5, "");		
		SetDlgItemText(hwnd, IDC_EDIT7, "");
		SendDlgItemMessage(hwnd, IDC_fotoo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);

	}

}
void MODIFICARMED(char Mod[], HWND hwnd, veterinario* nuevo)
{
	AuxV = inicioV;

	while (AuxV != nullptr && strcmp(Mod, AuxV->NOMBREV) != 0) {
		AuxV = AuxV->sigV;
	}

	if (AuxV == nullptr)
	{
	}
	else {

		strcpy_s(AuxV->NomU, nuevo->NomU);
		strcpy_s(AuxV->contrasena, nuevo->contrasena);
		strcpy_s(AuxV->CEDULA, nuevo->CEDULA);
		strcpy_s(AuxV->NOMBREV, nuevo->NOMBREV);
		strcpy_s(AuxV->FOTO, nuevo->FOTO);

		MessageBox(hwnd, "Usuario modificado", "AVISO", MB_OK | MB_ICONINFORMATION);

		SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_RESETCONTENT, 0, 0);
		AuxV = inicioV;
		while (AuxV != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)AuxV->NOMBREV);
			AuxV = AuxV->sigV;
		}

		EnableWindow(GetDlgItem(hwnd, IDC_EDIT2), false);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT3), false);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT5), false);
		EnableWindow(GetDlgItem(hwnd, IDC_EDIT7), false);

		ShowWindow(GetDlgItem(hwnd, IDC_BUTTON3), SW_HIDE);
		ShowWindow(GetDlgItem(hwnd, IDC_BUTTON4), SW_HIDE);
	}
}


void menu(WPARAM wParam, HWND hwnd)
{
	switch (LOWORD(wParam))
	{
	case ID_REGISTROS_USUARIOS:
	{
		EndDialog(hwnd, 0);
		DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG2), NULL, veterinarios);

	}break;

	case ID_REGISTROS_CITAS:
	{
		EndDialog(hwnd, 0);
		DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG4), NULL, CITAS);

	}break;

	case ID_MANEJODECITAS_AGENDA:
	{
		EndDialog(hwnd, 0);
		DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG5), NULL, AGENDA);

	}break;

	case ID_MEDICOS_INFORMACION:
	{
		EndDialog(hwnd, 0);
		DialogBox(HISNG, MAKEINTRESOURCE(IDD_DIALOG6), NULL, INFOMED);

	}break;



	
	}
}








