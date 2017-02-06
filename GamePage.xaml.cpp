//
// GamePage.xaml.cpp
// Implementation of the GamePage class
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "GamePage.xaml.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace ThatNumbersGame;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

/* constructor */
GamePage::GamePage() {

	InitializeComponent();

	// set up timer, delay = 100ms
	TimeSpan timeout;
	timeout.Duration = 100 * 1000 * 10;
	oDisplayTimer->Interval = timeout;
	oDisplayTimer->Tick += ref new EventHandler<Object^>(this, &GamePage::timerTick);

}

/* onClick event for next button */
void ThatNumbersGame::GamePage::btnNext_onClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e) {

	advanceState();

}

/* timer tick */
void ThatNumbersGame::GamePage::timerTick(Object^ sender, Object^ e) {

	oDisplayTimer->Stop();

	lblStatus->Text = "Enter the number you saw";
	txtAnswer->Text = "";

	btnNext->Visibility = Windows::UI::Xaml::Visibility::Visible;
	txtAnswer->Visibility = Windows::UI::Xaml::Visibility::Visible;
	lblNumber->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

	txtAnswer->Focus(Windows::UI::Xaml::FocusState::Programmatic);

	// change state
	eCurrentGameState = EGameState::ANSWERING;


}


void ThatNumbersGame::GamePage::txtAnswer_keyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e) {
	if (e->Key == Windows::System::VirtualKey::Enter) {
		advanceState();
	}
}

void ThatNumbersGame::GamePage::advanceState() {

	switch (eCurrentGameState) {

	case EGameState::WAITING:

		// generate number
		srand(time(NULL));

		{
			long long number = std::rand() % ((long long)std::pow(10, iNumDigits)) + (std::pow(10, iNumDigits - 1) - 1);
			sLastNumber = number.ToString();
			lblNumber->Text = number.ToString();
		}

		// update ui
		btnNext->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		lblNumber->Visibility = Windows::UI::Xaml::Visibility::Visible;

		// start timer
		oDisplayTimer->Start();

		// change state
		eCurrentGameState = EGameState::DISPLAYING;
		break;

	case EGameState::DISPLAYING:
		// shouldn't happen; button is invisible
		break;

	case EGameState::ANSWERING: {

			// check user's answer
			bool userCorrect = (txtAnswer->Text->Equals(sLastNumber));

			if (userCorrect) {

				// update ui
				lblStatus->Text = "Correct! Next: " + ++iNumDigits + " digits...";
				txtAnswer->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

				// change state
				eCurrentGameState = EGameState::WAITING;

			}
			else {

				// update ui
				Windows::UI::Xaml::Documents::Run^ line1 = ref new Windows::UI::Xaml::Documents::Run;
				line1->Text = "Aw no :(";
				Windows::UI::Xaml::Documents::Run^ line2 = ref new Windows::UI::Xaml::Documents::Run;
				line2->Text = "Final score: " + (iNumDigits - 1) + " digits.";
				lblStatus->Inlines->Clear();
				lblStatus->Inlines->Append(line1);
				lblStatus->Inlines->Append(ref new Windows::UI::Xaml::Documents::LineBreak);
				lblStatus->Inlines->Append(line2);

				txtAnswer->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

				iNumDigits = 1;

				// change state
				eCurrentGameState = EGameState::LOST;

			}
		}

		break;

	case LOST:
		Frame->Navigate(ThatNumbersGame::MainPage::typeid);
		break;

	}
}
