//
// GamePage.xaml.h
// Declaration of the GamePage class
//

#pragma once

#include "GamePage.g.h"

namespace ThatNumbersGame {
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class GamePage sealed {

	private:
		
		enum EGameState{ WAITING, DISPLAYING, ANSWERING, LOST};
		EGameState eCurrentGameState = EGameState::WAITING;

		int iNumDigits = 1;
		Platform::String^ sLastNumber = "";

		Windows::UI::Xaml::DispatcherTimer^ oDisplayTimer = ref new Windows::UI::Xaml::DispatcherTimer();
		
		void timerTick(Object^ sender, Object^ e);
		void advanceState();

	public:
		GamePage();

	private:
		void btnNext_onClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void txtAnswer_keyDown(Platform::Object^ sender, Windows::UI::Xaml::Input::KeyRoutedEventArgs^ e);
	};

}
