/*	
 *	AnyTone Frequency Offset Tool
 * 
 *	Written by: Lucas Burns
 * 
 */


#include <iostream>
#include "CSVFile.h"
#include "main.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	MainFrame* mainFrame = new MainFrame("AnyTone Frequency Offset Tool");
	mainFrame->SetSize(900, 800);
	mainFrame->Center();
	mainFrame->Show();

	return true;
}