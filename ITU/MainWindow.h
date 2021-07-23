#pragma once

#include "Images.h"
#include "Shutdown.h"

namespace ITU {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainWindow : public System::Windows::Forms::Form {
	public:
		MainWindow( void ) {
			InitializeComponent();
			images = ( gcnew System::ComponentModel::ComponentResourceManager( Images::typeid ) );

			// Nastavim promenne
			timeFormat = this->statusBar_actualTime->Text;
			offHour = System::DateTime::Now.Hour;
			offMinute = System::DateTime::Now.Minute;
			shutdown = new Shutdown;

			// Aktualizuji vsechny casy ve formulari
			updateActualTime();

			alignOffBoxes();
			offHourSet( offHour == 23 ? offHour = 0 : ++offHour );
			offMinuteSet( offMinute );

			// Prekreslim tlacitka
			redrawActionTypeButton();


		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainWindow() {
			if ( components ) {
				delete components;
			}
		}


	private:
		/* * My own variables * */
		
		String^ timeFormat;             // Formatovaci retezec pro zobrazeni data ve statsbaru a mozna i jinde?
		bool numberTyping = false;      // True, pokud uzivatel zrovna zapisuje cislo
		int offHour, offMinute;         // Cas vypnuti
		int mode;						// rezim vypnout v (0) / vypnout za (1)
		int offMinuteTmp, offHourTmp;	// usetreni problemu pri zpetnem prevodu
		DateTime^ shutdownTime;         // Cas vypnuti v podobe struktury
		bool mouseTimer_firstTick;
		int mouseTimer_action;
	public:
		ITU::Shutdown *shutdown;
		System::ComponentModel::ComponentResourceManager^ images;

		


		// Form sources
		private: System::ComponentModel::IContainer^  components;


		private: System::Windows::Forms::ToolStripStatusLabel^  statusBar_actualTime;
		private: System::Windows::Forms::TableLayoutPanel^  layout;
		private: System::Windows::Forms::StatusStrip^  statusBar;
		private: System::Windows::Forms::GroupBox^  offTimeBox;





		private: System::Windows::Forms::TableLayoutPanel^  offTimeBoxLayout;
		private: System::Windows::Forms::Button^  offTime_hourP;
		private: System::Windows::Forms::Label^  offTime_splitter;
		private: System::Windows::Forms::Button^  offTime_hourM;
		private: System::Windows::Forms::Button^  offTime_minuteP;
		private: System::Windows::Forms::Button^  offTime_minuteM;
		private: System::Windows::Forms::Timer^  halfSecondTimer;
		private: System::Windows::Forms::RichTextBox^  offTime_hour;
		private: System::Windows::Forms::RichTextBox^  offTime_minute;
		private: System::Windows::Forms::Timer^  shutdownTimer;
		private: System::Windows::Forms::ToolStripProgressBar^  statusBar_progress;
		private: System::Windows::Forms::Button^  setButton;
		private: System::Windows::Forms::Button^  actionTypeButton;
		private: System::Windows::Forms::ToolStripDropDownButton^  toolStripDropDownButton1;
		private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
		private: System::Windows::Forms::Button^  offTime_change;	
		private: System::Windows::Forms::Button^  offTime_reset;
		private: System::Windows::Forms::Button^  advanced;

	private: System::Windows::Forms::Timer^  mouseTimer;
	private: System::Windows::Forms::GroupBox^  actualTimeBox;
	private: System::Windows::Forms::TableLayoutPanel^  actualTimeBoxLayout;
	private: System::Windows::Forms::Label^  actualTime_hour;
	private: System::Windows::Forms::Label^  actualTime_splitter;
	private: System::Windows::Forms::Label^  actualTime_minute;





	private: System::Windows::Forms::Timer^  numberTypingTimer;
	

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent( void ) {
			this->components = ( gcnew System::ComponentModel::Container() );
			System::ComponentModel::ComponentResourceManager^  resources = ( gcnew System::ComponentModel::ComponentResourceManager( MainWindow::typeid ) );
			this->statusBar = ( gcnew System::Windows::Forms::StatusStrip() );
			this->statusBar_actualTime = ( gcnew System::Windows::Forms::ToolStripStatusLabel() );
			this->statusBar_progress = ( gcnew System::Windows::Forms::ToolStripProgressBar() );
			this->toolStripDropDownButton1 = ( gcnew System::Windows::Forms::ToolStripDropDownButton() );
			this->layout = ( gcnew System::Windows::Forms::TableLayoutPanel() );
			this->actualTimeBox = ( gcnew System::Windows::Forms::GroupBox() );
			this->actualTimeBoxLayout = ( gcnew System::Windows::Forms::TableLayoutPanel() );
			this->actualTime_hour = ( gcnew System::Windows::Forms::Label() );
			this->actualTime_splitter = ( gcnew System::Windows::Forms::Label() );
			this->actualTime_minute = ( gcnew System::Windows::Forms::Label() );
			this->offTimeBox = ( gcnew System::Windows::Forms::GroupBox() );
			this->offTimeBoxLayout = ( gcnew System::Windows::Forms::TableLayoutPanel() );
			this->offTime_minute = ( gcnew System::Windows::Forms::RichTextBox() );
			this->offTime_hourM = ( gcnew System::Windows::Forms::Button() );
			this->offTime_hourP = ( gcnew System::Windows::Forms::Button() );
			this->offTime_hour = ( gcnew System::Windows::Forms::RichTextBox() );
			this->offTime_splitter = ( gcnew System::Windows::Forms::Label() );
			this->offTime_minuteP = ( gcnew System::Windows::Forms::Button() );
			this->offTime_minuteM = ( gcnew System::Windows::Forms::Button() );
			this->setButton = ( gcnew System::Windows::Forms::Button() );
			this->actionTypeButton = ( gcnew System::Windows::Forms::Button() );
			this->tableLayoutPanel1 = ( gcnew System::Windows::Forms::TableLayoutPanel() );
			this->offTime_change = ( gcnew System::Windows::Forms::Button() );
			this->offTime_reset = ( gcnew System::Windows::Forms::Button() );
			this->advanced = ( gcnew System::Windows::Forms::Button() );
			this->halfSecondTimer = ( gcnew System::Windows::Forms::Timer( this->components ) );
			this->numberTypingTimer = ( gcnew System::Windows::Forms::Timer( this->components ) );
			this->shutdownTimer = ( gcnew System::Windows::Forms::Timer( this->components ) );
			this->mouseTimer = ( gcnew System::Windows::Forms::Timer( this->components ) );
			this->statusBar->SuspendLayout();
			this->layout->SuspendLayout();
			this->actualTimeBox->SuspendLayout();
			this->actualTimeBoxLayout->SuspendLayout();
			this->offTimeBox->SuspendLayout();
			this->offTimeBoxLayout->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// statusBar
			// 
			this->statusBar->ImageScalingSize = System::Drawing::Size( 20, 20 );
			this->statusBar->Items->AddRange( gcnew cli::array< System::Windows::Forms::ToolStripItem^  >( 3 ) {
				this->statusBar_actualTime,
					this->statusBar_progress, this->toolStripDropDownButton1
			} );
			this->statusBar->LayoutStyle = System::Windows::Forms::ToolStripLayoutStyle::HorizontalStackWithOverflow;
			this->statusBar->Location = System::Drawing::Point( 0, 386 );
			this->statusBar->Name = L"statusBar";
			this->statusBar->Size = System::Drawing::Size( 642, 26 );
			this->statusBar->SizingGrip = false;
			this->statusBar->TabIndex = 0;
			this->statusBar->Text = L"statusStrip1";
			// 
			// statusBar_actualTime
			// 
			this->statusBar_actualTime->Name = L"statusBar_actualTime";
			this->statusBar_actualTime->Size = System::Drawing::Size( 179, 21 );
			this->statusBar_actualTime->Text = L"d. MMMM yyyy HH:mm:ss";
			// 
			// statusBar_progress
			// 
			this->statusBar_progress->Name = L"statusBar_progress";
			this->statusBar_progress->Size = System::Drawing::Size( 100, 20 );
			this->statusBar_progress->Step = 1;
			this->statusBar_progress->ToolTipText = L"K vypnutí dojde za xx m v hh:mm";
			// 
			// toolStripDropDownButton1
			// 
			this->toolStripDropDownButton1->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->toolStripDropDownButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Text;
			this->toolStripDropDownButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripDropDownButton1->Name = L"toolStripDropDownButton1";
			this->toolStripDropDownButton1->Size = System::Drawing::Size( 65, 24 );
			this->toolStripDropDownButton1->Text = L"Profily";
			// 
			// layout
			// 
			this->layout->ColumnCount = 4;
			this->layout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Absolute, 235 ) ) );
			this->layout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Absolute, 200 ) ) );
			this->layout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Absolute, 200 ) ) );
			this->layout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle() ) );
			this->layout->Controls->Add( this->actualTimeBox, 0, 0 );
			this->layout->Controls->Add( this->offTimeBox, 0, 1 );
			this->layout->Controls->Add( this->setButton, 2, 1 );
			this->layout->Controls->Add( this->actionTypeButton, 1, 1 );
			this->layout->Controls->Add( this->tableLayoutPanel1, 0, 2 );
			this->layout->Controls->Add( this->advanced, 2, 2 );
			this->layout->Dock = System::Windows::Forms::DockStyle::Fill;
			this->layout->Location = System::Drawing::Point( 0, 0 );
			this->layout->Name = L"layout";
			this->layout->RowCount = 4;
			this->layout->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle( System::Windows::Forms::SizeType::Absolute, 100 ) ) );
			this->layout->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle( System::Windows::Forms::SizeType::Absolute, 200 ) ) );
			this->layout->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle( System::Windows::Forms::SizeType::Absolute, 80 ) ) );
			this->layout->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle() ) );
			this->layout->Size = System::Drawing::Size( 642, 386 );
			this->layout->TabIndex = 0;
			// 
			// actualTimeBox
			// 
			this->actualTimeBox->Controls->Add( this->actualTimeBoxLayout );
			this->actualTimeBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->actualTimeBox->Location = System::Drawing::Point( 3, 3 );
			this->actualTimeBox->Name = L"actualTimeBox";
			this->actualTimeBox->Size = System::Drawing::Size( 229, 94 );
			this->actualTimeBox->TabIndex = 0;
			this->actualTimeBox->TabStop = false;
			this->actualTimeBox->Text = L"Aktuální èas";
			// 
			// actualTimeBoxLayout
			// 
			this->actualTimeBoxLayout->ColumnCount = 4;
			this->actualTimeBoxLayout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Absolute,
				90 ) ) );
			this->actualTimeBoxLayout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Absolute,
				40 ) ) );
			this->actualTimeBoxLayout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Absolute,
				90 ) ) );
			this->actualTimeBoxLayout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle() ) );
			this->actualTimeBoxLayout->Controls->Add( this->actualTime_hour, 0, 0 );
			this->actualTimeBoxLayout->Controls->Add( this->actualTime_splitter, 1, 0 );
			this->actualTimeBoxLayout->Controls->Add( this->actualTime_minute, 2, 0 );
			this->actualTimeBoxLayout->Dock = System::Windows::Forms::DockStyle::Fill;
			this->actualTimeBoxLayout->Location = System::Drawing::Point( 3, 18 );
			this->actualTimeBoxLayout->Name = L"actualTimeBoxLayout";
			this->actualTimeBoxLayout->RowCount = 1;
			this->actualTimeBoxLayout->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle( System::Windows::Forms::SizeType::Percent,
				100 ) ) );
			this->actualTimeBoxLayout->Size = System::Drawing::Size( 223, 73 );
			this->actualTimeBoxLayout->TabIndex = 0;
			// 
			// actualTime_hour
			// 
			this->actualTime_hour->AutoSize = true;
			this->actualTime_hour->Dock = System::Windows::Forms::DockStyle::Fill;
			this->actualTime_hour->Font = ( gcnew System::Drawing::Font( L"Microsoft Sans Serif", 30 ) );
			this->actualTime_hour->Location = System::Drawing::Point( 3, 0 );
			this->actualTime_hour->Name = L"actualTime_hour";
			this->actualTime_hour->Size = System::Drawing::Size( 84, 73 );
			this->actualTime_hour->TabIndex = 0;
			this->actualTime_hour->Text = L"10";
			this->actualTime_hour->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// actualTime_splitter
			// 
			this->actualTime_splitter->AutoSize = true;
			this->actualTime_splitter->Dock = System::Windows::Forms::DockStyle::Fill;
			this->actualTime_splitter->Font = ( gcnew System::Drawing::Font( L"Microsoft Sans Serif", 30 ) );
			this->actualTime_splitter->ForeColor = System::Drawing::SystemColors::WindowText;
			this->actualTime_splitter->Location = System::Drawing::Point( 93, 0 );
			this->actualTime_splitter->Name = L"actualTime_splitter";
			this->actualTime_splitter->Size = System::Drawing::Size( 34, 73 );
			this->actualTime_splitter->TabIndex = 0;
			this->actualTime_splitter->Text = L":";
			this->actualTime_splitter->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// actualTime_minute
			// 
			this->actualTime_minute->AutoSize = true;
			this->actualTime_minute->Dock = System::Windows::Forms::DockStyle::Fill;
			this->actualTime_minute->Font = ( gcnew System::Drawing::Font( L"Microsoft Sans Serif", 30 ) );
			this->actualTime_minute->Location = System::Drawing::Point( 133, 0 );
			this->actualTime_minute->Name = L"actualTime_minute";
			this->actualTime_minute->Size = System::Drawing::Size( 84, 73 );
			this->actualTime_minute->TabIndex = 0;
			this->actualTime_minute->Text = L"57";
			this->actualTime_minute->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// offTimeBox
			// 
			this->offTimeBox->Controls->Add( this->offTimeBoxLayout );
			this->offTimeBox->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTimeBox->Location = System::Drawing::Point( 3, 103 );
			this->offTimeBox->Name = L"offTimeBox";
			this->offTimeBox->Size = System::Drawing::Size( 229, 194 );
			this->offTimeBox->TabIndex = 0;
			this->offTimeBox->TabStop = false;
			this->offTimeBox->Text = L"Vypnout v";
			// 
			// offTimeBoxLayout
			// 
			this->offTimeBoxLayout->ColumnCount = 4;
			this->offTimeBoxLayout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Absolute,
				90 ) ) );
			this->offTimeBoxLayout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Absolute,
				40 ) ) );
			this->offTimeBoxLayout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Absolute,
				90 ) ) );
			this->offTimeBoxLayout->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle() ) );
			this->offTimeBoxLayout->Controls->Add( this->offTime_minute, 2, 1 );
			this->offTimeBoxLayout->Controls->Add( this->offTime_hourM, 0, 2 );
			this->offTimeBoxLayout->Controls->Add( this->offTime_hourP, 0, 0 );
			this->offTimeBoxLayout->Controls->Add( this->offTime_hour, 0, 1 );
			this->offTimeBoxLayout->Controls->Add( this->offTime_splitter, 1, 1 );
			this->offTimeBoxLayout->Controls->Add( this->offTime_minuteP, 2, 0 );
			this->offTimeBoxLayout->Controls->Add( this->offTime_minuteM, 2, 2 );
			this->offTimeBoxLayout->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTimeBoxLayout->Location = System::Drawing::Point( 3, 18 );
			this->offTimeBoxLayout->Name = L"offTimeBoxLayout";
			this->offTimeBoxLayout->RowCount = 4;
			this->offTimeBoxLayout->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle( System::Windows::Forms::SizeType::Absolute, 60 ) ) );
			this->offTimeBoxLayout->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle( System::Windows::Forms::SizeType::Absolute, 60 ) ) );
			this->offTimeBoxLayout->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle( System::Windows::Forms::SizeType::Absolute, 60 ) ) );
			this->offTimeBoxLayout->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle() ) );
			this->offTimeBoxLayout->Size = System::Drawing::Size( 223, 173 );
			this->offTimeBoxLayout->TabIndex = 0;
			// 
			// offTime_minute
			// 
			this->offTime_minute->BackColor = System::Drawing::SystemColors::Control;
			this->offTime_minute->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->offTime_minute->DetectUrls = false;
			this->offTime_minute->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTime_minute->Font = ( gcnew System::Drawing::Font( L"Microsoft Sans Serif", 30, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast< System::Byte >( 238 ) ) );
			this->offTime_minute->Location = System::Drawing::Point( 133, 63 );
			this->offTime_minute->MaxLength = 2;
			this->offTime_minute->Multiline = false;
			this->offTime_minute->Name = L"offTime_minute";
			this->offTime_minute->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::None;
			this->offTime_minute->ShortcutsEnabled = false;
			this->offTime_minute->Size = System::Drawing::Size( 84, 54 );
			this->offTime_minute->TabIndex = 2;
			this->offTime_minute->Text = L"57";
			this->offTime_minute->WordWrap = false;
			this->offTime_minute->KeyDown += gcnew System::Windows::Forms::KeyEventHandler( this, &MainWindow::offTime_minute_KeyPress );
			this->offTime_minute->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler( this, &MainWindow::offTime_minute_KeyPress );
			// 
			// offTime_hourM
			// 
			this->offTime_hourM->Cursor = System::Windows::Forms::Cursors::Hand;
			this->offTime_hourM->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTime_hourM->FlatAppearance->BorderSize = 0;
			this->offTime_hourM->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::Control;
			this->offTime_hourM->FlatAppearance->MouseOverBackColor = System::Drawing::SystemColors::Control;
			this->offTime_hourM->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->offTime_hourM->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"offTime_hourM.Image" ) ) );
			this->offTime_hourM->ImageAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->offTime_hourM->Location = System::Drawing::Point( 3, 123 );
			this->offTime_hourM->Name = L"offTime_hourM";
			this->offTime_hourM->Size = System::Drawing::Size( 84, 54 );
			this->offTime_hourM->TabIndex = 0;
			this->offTime_hourM->TabStop = false;
			this->offTime_hourM->UseVisualStyleBackColor = false;
			this->offTime_hourM->Click += gcnew System::EventHandler( this, &MainWindow::offTime_hourM_Click );
			this->offTime_hourM->MouseDown += gcnew System::Windows::Forms::MouseEventHandler( this, &MainWindow::offTime_hourM_MouseDown );
			this->offTime_hourM->MouseLeave += gcnew System::EventHandler( this, &MainWindow::offTime_hourM_MouseLeave );
			this->offTime_hourM->MouseUp += gcnew System::Windows::Forms::MouseEventHandler( this, &MainWindow::offTime_hourM_MouseUp );
			// 
			// offTime_hourP
			// 
			this->offTime_hourP->Cursor = System::Windows::Forms::Cursors::Hand;
			this->offTime_hourP->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTime_hourP->FlatAppearance->BorderSize = 0;
			this->offTime_hourP->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::Control;
			this->offTime_hourP->FlatAppearance->MouseOverBackColor = System::Drawing::SystemColors::Control;
			this->offTime_hourP->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->offTime_hourP->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"offTime_hourP.Image" ) ) );
			this->offTime_hourP->ImageAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->offTime_hourP->Location = System::Drawing::Point( 3, 3 );
			this->offTime_hourP->Name = L"offTime_hourP";
			this->offTime_hourP->Size = System::Drawing::Size( 84, 54 );
			this->offTime_hourP->TabIndex = 0;
			this->offTime_hourP->TabStop = false;
			this->offTime_hourP->UseVisualStyleBackColor = false;
			this->offTime_hourP->Click += gcnew System::EventHandler( this, &MainWindow::offTime_hourP_Click );
			this->offTime_hourP->MouseDown += gcnew System::Windows::Forms::MouseEventHandler( this, &MainWindow::offTime_hourP_MouseDown );
			this->offTime_hourP->MouseLeave += gcnew System::EventHandler( this, &MainWindow::offTime_hourP_MouseLeave );
			this->offTime_hourP->MouseUp += gcnew System::Windows::Forms::MouseEventHandler( this, &MainWindow::offTime_hourP_MouseUp );
			// 
			// offTime_hour
			// 
			this->offTime_hour->BackColor = System::Drawing::SystemColors::Control;
			this->offTime_hour->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->offTime_hour->DetectUrls = false;
			this->offTime_hour->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTime_hour->Font = ( gcnew System::Drawing::Font( L"Microsoft Sans Serif", 30, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast< System::Byte >( 238 ) ) );
			this->offTime_hour->Location = System::Drawing::Point( 3, 63 );
			this->offTime_hour->MaxLength = 2;
			this->offTime_hour->Multiline = false;
			this->offTime_hour->Name = L"offTime_hour";
			this->offTime_hour->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::None;
			this->offTime_hour->ShortcutsEnabled = false;
			this->offTime_hour->Size = System::Drawing::Size( 84, 54 );
			this->offTime_hour->TabIndex = 1;
			this->offTime_hour->Text = L"11";
			this->offTime_hour->WordWrap = false;
			this->offTime_hour->KeyDown += gcnew System::Windows::Forms::KeyEventHandler( this, &MainWindow::offTime_hour_KeyPress );
			this->offTime_hour->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler( this, &MainWindow::offTime_hour_KeyPress );
			// 
			// offTime_splitter
			// 
			this->offTime_splitter->AutoSize = true;
			this->offTime_splitter->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTime_splitter->Font = ( gcnew System::Drawing::Font( L"Microsoft Sans Serif", 30 ) );
			this->offTime_splitter->Location = System::Drawing::Point( 93, 60 );
			this->offTime_splitter->Name = L"offTime_splitter";
			this->offTime_splitter->Size = System::Drawing::Size( 34, 60 );
			this->offTime_splitter->TabIndex = 0;
			this->offTime_splitter->Text = L":";
			this->offTime_splitter->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// offTime_minuteP
			// 
			this->offTime_minuteP->Cursor = System::Windows::Forms::Cursors::Hand;
			this->offTime_minuteP->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTime_minuteP->FlatAppearance->BorderSize = 0;
			this->offTime_minuteP->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::Control;
			this->offTime_minuteP->FlatAppearance->MouseOverBackColor = System::Drawing::SystemColors::Control;
			this->offTime_minuteP->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->offTime_minuteP->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"offTime_minuteP.Image" ) ) );
			this->offTime_minuteP->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->offTime_minuteP->Location = System::Drawing::Point( 133, 3 );
			this->offTime_minuteP->Name = L"offTime_minuteP";
			this->offTime_minuteP->Size = System::Drawing::Size( 84, 54 );
			this->offTime_minuteP->TabIndex = 0;
			this->offTime_minuteP->TabStop = false;
			this->offTime_minuteP->UseVisualStyleBackColor = false;
			this->offTime_minuteP->Click += gcnew System::EventHandler( this, &MainWindow::offTime_minuteP_Click );
			this->offTime_minuteP->MouseDown += gcnew System::Windows::Forms::MouseEventHandler( this, &MainWindow::offTime_minuteP_MouseDown );
			this->offTime_minuteP->MouseLeave += gcnew System::EventHandler( this, &MainWindow::offTime_minuteP_MouseLeave );
			this->offTime_minuteP->MouseUp += gcnew System::Windows::Forms::MouseEventHandler( this, &MainWindow::offTime_minuteP_MouseUp );
			// 
			// offTime_minuteM
			// 
			this->offTime_minuteM->Cursor = System::Windows::Forms::Cursors::Hand;
			this->offTime_minuteM->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTime_minuteM->FlatAppearance->BorderSize = 0;
			this->offTime_minuteM->FlatAppearance->MouseDownBackColor = System::Drawing::SystemColors::Control;
			this->offTime_minuteM->FlatAppearance->MouseOverBackColor = System::Drawing::SystemColors::Control;
			this->offTime_minuteM->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->offTime_minuteM->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"offTime_minuteM.Image" ) ) );
			this->offTime_minuteM->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->offTime_minuteM->Location = System::Drawing::Point( 133, 123 );
			this->offTime_minuteM->Name = L"offTime_minuteM";
			this->offTime_minuteM->Size = System::Drawing::Size( 84, 54 );
			this->offTime_minuteM->TabIndex = 0;
			this->offTime_minuteM->TabStop = false;
			this->offTime_minuteM->UseVisualStyleBackColor = false;
			this->offTime_minuteM->Click += gcnew System::EventHandler( this, &MainWindow::offTime_minuteM_Click );
			this->offTime_minuteM->MouseDown += gcnew System::Windows::Forms::MouseEventHandler( this, &MainWindow::offTime_minuteM_MouseDown );
			this->offTime_minuteM->MouseLeave += gcnew System::EventHandler( this, &MainWindow::offTime_minuteM_MouseLeave );
			this->offTime_minuteM->MouseUp += gcnew System::Windows::Forms::MouseEventHandler( this, &MainWindow::offTime_minuteM_MouseUp );
			// 
			// setButton
			// 
			this->setButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->setButton->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"setButton.Image" ) ) );
			this->setButton->Location = System::Drawing::Point( 465, 130 );
			this->setButton->Margin = System::Windows::Forms::Padding( 30 );
			this->setButton->Name = L"setButton";
			this->setButton->Size = System::Drawing::Size( 140, 140 );
			this->setButton->TabIndex = 4;
			this->setButton->Click += gcnew System::EventHandler( this, &MainWindow::setButton_Click );
			// 
			// actionTypeButton
			// 
			this->actionTypeButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->actionTypeButton->Dock = System::Windows::Forms::DockStyle::Fill;
			this->actionTypeButton->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"actionTypeButton.Image" ) ) );
			this->actionTypeButton->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->actionTypeButton->Location = System::Drawing::Point( 265, 130 );
			this->actionTypeButton->Margin = System::Windows::Forms::Padding( 30 );
			this->actionTypeButton->Name = L"actionTypeButton";
			this->actionTypeButton->Padding = System::Windows::Forms::Padding( 20 );
			this->actionTypeButton->Size = System::Drawing::Size( 140, 140 );
			this->actionTypeButton->TabIndex = 3;
			this->actionTypeButton->Text = L"Vypnout";
			this->actionTypeButton->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			this->actionTypeButton->UseVisualStyleBackColor = true;
			this->actionTypeButton->Click += gcnew System::EventHandler( this, &MainWindow::actionTypeButton_Click );
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 3;
			this->tableLayoutPanel1->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Percent,
				33.33333F ) ) );
			this->tableLayoutPanel1->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Percent,
				33.33333F ) ) );
			this->tableLayoutPanel1->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Percent,
				33.33333F ) ) );
			this->tableLayoutPanel1->ColumnStyles->Add( ( gcnew System::Windows::Forms::ColumnStyle( System::Windows::Forms::SizeType::Absolute,
				20 ) ) );
			this->tableLayoutPanel1->Controls->Add( this->offTime_change, 2, 0 );
			this->tableLayoutPanel1->Controls->Add( this->offTime_reset, 0, 0 );
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point( 3, 303 );
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 1;
			this->tableLayoutPanel1->RowStyles->Add( ( gcnew System::Windows::Forms::RowStyle( System::Windows::Forms::SizeType::Percent, 100 ) ) );
			this->tableLayoutPanel1->Size = System::Drawing::Size( 229, 74 );
			this->tableLayoutPanel1->TabIndex = 5;
			// 
			// offTime_change
			// 
			this->offTime_change->Cursor = System::Windows::Forms::Cursors::Hand;
			this->offTime_change->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTime_change->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"offTime_change.Image" ) ) );
			this->offTime_change->Location = System::Drawing::Point( 155, 3 );
			this->offTime_change->Name = L"offTime_change";
			this->offTime_change->Size = System::Drawing::Size( 71, 68 );
			this->offTime_change->TabIndex = 7;
			this->offTime_change->UseVisualStyleBackColor = true;
			this->offTime_change->Click += gcnew System::EventHandler( this, &MainWindow::offTime_change_Click );
			// 
			// offTime_reset
			// 
			this->offTime_reset->Cursor = System::Windows::Forms::Cursors::Hand;
			this->offTime_reset->Dock = System::Windows::Forms::DockStyle::Fill;
			this->offTime_reset->Image = ( cli::safe_cast< System::Drawing::Image^ >( resources->GetObject( L"offTime_reset.Image" ) ) );
			this->offTime_reset->Location = System::Drawing::Point( 3, 3 );
			this->offTime_reset->Name = L"offTime_reset";
			this->offTime_reset->Size = System::Drawing::Size( 70, 68 );
			this->offTime_reset->TabIndex = 8;
			this->offTime_reset->UseVisualStyleBackColor = true;
			this->offTime_reset->Click += gcnew System::EventHandler( this, &MainWindow::offTime_reset_Click );
			// 
			// advanced
			// 
			this->advanced->Cursor = System::Windows::Forms::Cursors::Hand;
			this->advanced->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->advanced->Enabled = false;
			this->advanced->Location = System::Drawing::Point( 438, 354 );
			this->advanced->Name = L"advanced";
			this->advanced->Size = System::Drawing::Size( 194, 23 );
			this->advanced->TabIndex = 5;
			this->advanced->Text = L"Pokroèilé možnosti";
			this->advanced->UseVisualStyleBackColor = true;
			// 
			// halfSecondTimer
			// 
			this->halfSecondTimer->Enabled = true;
			this->halfSecondTimer->Interval = 500;
			this->halfSecondTimer->Tick += gcnew System::EventHandler( this, &MainWindow::halfSecondTimer_Tick );
			// 
			// numberTypingTimer
			// 
			this->numberTypingTimer->Interval = 2000;
			this->numberTypingTimer->Tick += gcnew System::EventHandler( this, &MainWindow::numberTypingTimer_Tick );
			// 
			// shutdownTimer
			// 
			this->shutdownTimer->Tick += gcnew System::EventHandler( this, &MainWindow::shutdownTimer_Tick );
			// 
			// mouseTimer
			// 
			this->mouseTimer->Interval = 2000;
			this->mouseTimer->Tick += gcnew System::EventHandler( this, &MainWindow::mouseTimer_Tick );
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF( 8, 16 );
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size( 642, 412 );
			this->Controls->Add( this->layout );
			this->Controls->Add( this->statusBar );
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Icon = ( cli::safe_cast< System::Drawing::Icon^ >( resources->GetObject( L"$this.Icon" ) ) );
			this->MaximizeBox = false;
			this->Name = L"MainWindow";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->Text = L"Vypnutí PC";
			this->EnabledChanged += gcnew System::EventHandler( this, &MainWindow::MainWindow_EnabledChanged );
			this->statusBar->ResumeLayout( false );
			this->statusBar->PerformLayout();
			this->layout->ResumeLayout( false );
			this->actualTimeBox->ResumeLayout( false );
			this->actualTimeBoxLayout->ResumeLayout( false );
			this->actualTimeBoxLayout->PerformLayout();
			this->offTimeBox->ResumeLayout( false );
			this->offTimeBoxLayout->ResumeLayout( false );
			this->offTimeBoxLayout->PerformLayout();
			this->tableLayoutPanel1->ResumeLayout( false );
			this->ResumeLayout( false );
			this->PerformLayout();

		}
#pragma endregion
	private:
		// My own metods
		void updateActualTime ();
		void alignOffBoxes ();
		void redrawActionTypeButton ();
		
		void offHourSet    ( int );
		void offHourSet    ( Decimal );
		void offHourSet    ( String^ );
		void offHourPlus   ();
		void offHourMinus  ();
		void offHourUpdate ();
		
		void offMinuteSet    ( int );
		void offMinuteSet    ( int, bool setButtonHour );
		void offMinuteSet    ( Decimal );
		void offMinuteSet    ( String^ );
		void offMinutePlus   ();
		void offMinuteMinus  ();
		void offMinuteUpdate ();

		void startMouseTimer ( int action );
		void stopMouseTimer ();


		//Form metods
		Void halfSecondTimer_Tick       ( Object^ sender, EventArgs^ e );
		Void offTime_hourP_Click        ( Object^ sender, EventArgs^ e );
		Void offTime_hourM_Click        ( Object^ sender, EventArgs^ e );
		Void offTime_minuteP_Click      ( Object^ sender, EventArgs^ e );
		Void offTime_minuteM_Click      ( Object^ sender, EventArgs^ e );
		Void offTime_hour_KeyPress      ( Object^ sender, KeyEventArgs^ e );
		Void offTime_hour_KeyPress      ( Object^ sender, KeyPressEventArgs^ e );
		Void offTime_minute_KeyPress    ( Object^ sender, KeyEventArgs^ e );
		Void offTime_minute_KeyPress    ( Object^ sender, KeyPressEventArgs^ e );
		Void numberTypingTimer_Tick     ( Object^ sender, EventArgs^ e );
		Void shutdownTimer_Tick         ( Object^ sender, EventArgs^ e );
		Void setButton_Click            ( Object^ sender, EventArgs^ e );
		Void offTime_reset_Click        ( Object^ sender, EventArgs^ e );
		Void actionTypeButton_Click     ( Object^ sender, EventArgs^ e );
		Void MainWindow_EnabledChanged  ( Object^ sender, EventArgs^ e );
		Void mouseTimer_Tick            ( Object^ sender, EventArgs^ e );
		Void offTime_hourP_MouseDown    ( Object^ sender, MouseEventArgs^ e ) { startMouseTimer( 1 ); }
		Void offTime_hourP_MouseUp      ( Object^ sender, MouseEventArgs^ e ) { stopMouseTimer(); }
		Void offTime_hourP_MouseLeave   ( Object^ sender, EventArgs^ e )      { stopMouseTimer(); }
		Void offTime_hourM_MouseDown    ( Object^ sender, MouseEventArgs^ e ) { startMouseTimer( 2 ); }
		Void offTime_hourM_MouseUp      ( Object^ sender, MouseEventArgs^ e ) { stopMouseTimer(); }
		Void offTime_hourM_MouseLeave   ( Object^ sender, EventArgs^ e )      { stopMouseTimer(); }
		Void offTime_minuteP_MouseDown  ( Object^ sender, MouseEventArgs^ e ) { startMouseTimer( 3 ); }
		Void offTime_minuteP_MouseUp    ( Object^ sender, MouseEventArgs^ e ) { stopMouseTimer(); }
		Void offTime_minuteP_MouseLeave ( Object^ sender, EventArgs^ e )      { stopMouseTimer(); }
		Void offTime_minuteM_MouseDown  ( Object^ sender, MouseEventArgs^ e ) { startMouseTimer( 4 ); }
		Void offTime_minuteM_MouseUp    ( Object^ sender, MouseEventArgs^ e ) { stopMouseTimer(); }
		Void offTime_minuteM_MouseLeave ( Object^ sender, EventArgs^ e )      { stopMouseTimer(); }
		Void offTime_change_Click       ( Object^ sender, EventArgs^ e );
};
}
