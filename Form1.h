#pragma once

namespace MathArmy {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	struct bestP_and_chosen_tz  //создаем структуру для хранения значения максимального Р и номера задания
	{
		float bestP;
		int chosen_tz;
	};
	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:

		int k; //Переменная для количества групп k
		int tz_num; //Переменная для количества заданий tz_num
		int *Sk;		//Создаем массивы для количества в группах УЭ
		int *Ck;	//и значений их важности
		int *Ai; // Создаем массив для числа элементов заданий каждого вида
		float **P; //Создаем массив для значенйи вероятностей
		float **q; //Создаем массив для значений q
		int **X; //Создаем массив для значений X
		float *best_P; //Создаем массив для лучших значений P для каждой из групп
		int *chosen_tz; //Содаем массив для хранения номеров выбранных заданий
		float *delta_mas; //Создаем массив для дельт

		float U; //Переменная полноты контроля
		float E;//Переменная эффективности


		//Описание функций
		void Data_Enter_step1(void)
		{	
			k = 3;
			tz_num = numericUpDown1->Value.ToInt32(numericUpDown1->Value);
		}//Функция ввода числа групп и заданий

		void Data_Enter_step2(void)
		{
			//Заполнение значений Sk
			Sk[0] = System::Convert::ToInt32(textBox1->Text);
			Sk[1] = System::Convert::ToInt32(textBox2->Text);
			Sk[2] = System::Convert::ToInt32(textBox3->Text);

			//Заполннеие значений Ck
			Ck[0] = System::Convert::ToInt32(textBox4->Text);
			Ck[1] = System::Convert::ToInt32(textBox5->Text);
			Ck[2] = System::Convert::ToInt32(textBox6->Text);

			//Заполнение значений Ai
			Ai[0] = System::Convert::ToInt32(textBox25->Text);
			Ai[1] = System::Convert::ToInt32(textBox26->Text);
			if (numericUpDown1->Value.ToInt32(numericUpDown1->Value)>2)
				Ai[2] = System::Convert::ToInt32(textBox27->Text);
			if (numericUpDown1->Value.ToInt32(numericUpDown1->Value)>3)
				Ai[3] = System::Convert::ToInt32(textBox28->Text);
			if (numericUpDown1->Value.ToInt32(numericUpDown1->Value)>4)
				Ai[4] = System::Convert::ToInt32(textBox29->Text);
			if (numericUpDown1->Value.ToInt32(numericUpDown1->Value)>5)
				Ai[5] = System::Convert::ToInt32(textBox30->Text);
		}// Функция ввода числа элементов в группе, стоимости, числа элементов в заданиях


		void Data_Enter_step3(void)
		{
			int i;
			int j;
			for (i = 0; i < tz_num; i++)
			{
				P[i] = new float[k];
				q[i] = new float[k];
				X[i] = new int[k];
			}
			P[0][0] = System::Convert::ToSingle(textBox7->Text);
			P[0][1] = System::Convert::ToSingle(textBox8->Text);
			P[0][2] = System::Convert::ToSingle(textBox9->Text);
			P[1][0] = System::Convert::ToSingle(textBox10->Text);
			P[1][1] = System::Convert::ToSingle(textBox11->Text);
			P[1][2] = System::Convert::ToSingle(textBox12->Text);
			if (numericUpDown1->Value.ToInt32(numericUpDown1->Value)>2)
			{
				P[2][0] = System::Convert::ToSingle(textBox13->Text);
				P[2][1] = System::Convert::ToSingle(textBox14->Text);
				P[2][2] = System::Convert::ToSingle(textBox15->Text);
			}
			if (numericUpDown1->Value.ToInt32(numericUpDown1->Value)>3)
			{
				P[3][0] = System::Convert::ToSingle(textBox16->Text);
				P[3][1] = System::Convert::ToSingle(textBox17->Text);
				P[3][2] = System::Convert::ToSingle(textBox18->Text);
			}
			if (numericUpDown1->Value.ToInt32(numericUpDown1->Value)>4)
			{
				P[4][0] = System::Convert::ToSingle(textBox19->Text);
				P[4][1] = System::Convert::ToSingle(textBox20->Text);
				P[4][2] = System::Convert::ToSingle(textBox21->Text);
			}
			if (numericUpDown1->Value.ToInt32(numericUpDown1->Value)>5)
			{
				P[5][0] = System::Convert::ToSingle(textBox22->Text);
				P[5][1] = System::Convert::ToSingle(textBox23->Text);
				P[5][2] = System::Convert::ToSingle(textBox24->Text);
			}

			for (i = 0; i < tz_num; i++)
			{
				for (j = 0; j < k; j++)
				{
					q[i][j] = (1 - P[i][j]);//Заполняем q

					X[i][j] = 0; //Заполняем начальные Х
				}

			}
		}// Функция создания и ввода массива вероятностей P, подсчета q, заполнения X нулями


		void Fill_bestP_and_chosentz(void)
		{
			int i;
			bestP_and_chosen_tz Ptz;
			for (i = 0; i < k; i++)
			{
				Ptz = Find_bestP(i);
				best_P[i] = Ptz.bestP;
				chosen_tz[i] = Ptz.chosen_tz;
			}
		} // Функция начального подсчета лучших P для каждой из групп


		bestP_and_chosen_tz Find_bestP(int k)
		{
			int i;
			bestP_and_chosen_tz Ptz;
			Ptz.bestP = P[0][k];
			Ptz.chosen_tz = 0;
			for (i = 1; i < tz_num; i++)
			{
				if (P[i][k] > Ptz.bestP)
				{
					Ptz.bestP = P[i][k];
					Ptz.chosen_tz = i;
				}
			}
			return Ptz;
		} // Функция подсчета лучшего P для выбранной группы


		float delta_count(int C, float P)
		{
			float delta;
			delta = (C * P);
			return delta;
		} //Функция для подсчета начальной дельты
		float delta_count(float old_delta, float q)
		{
			float delta;
			delta = (old_delta * q);
			return delta;
		} //Функция для пересчета дельты после успешной выдачи заданий для группы
		float delta_count(float old_delta, float oldP, float newP)
		{
			float delta;
			delta = (old_delta * (newP / oldP));
			return delta;
		} //Функция для пересчета дельты после того, как закончился один тип заданий и был выбран другой

		void First_delta_count(void)
		{
			int i;
			for (i = 0; i < k; i++)
			{

				delta_mas[i] = delta_count(Ck[i], best_P[i]);
			}
		} //Функция для подсчета начальных дельт для выбранных начальных P
		int MaxDelta(void)
		{
			int i;
			int index = 0;
			float MaxDelta = delta_mas[0];
			int Cold=0;
			for (i = 1; i < k; i++)
			{
				if (delta_mas[i] > MaxDelta)
				{
					MaxDelta = delta_mas[i];
					index = i;
					Cold = i;
				}
				if (delta_mas[i] == MaxDelta)
				{
					if (Ck[i] > Cold)
					{
						MaxDelta = delta_mas[i];
						index = i;
						Cold = i;
					}
				}



			}

			return index;
		} //Функция выбора максимальной дельты для работы
		bool IsTasksThere(int MaxDeltaNumber)
		{
			int TaskNum = chosen_tz[MaxDeltaNumber];
			bool CanBeUsed = 0;

			if (Ai[TaskNum] <= 0)
				return 0;
			else
			{
				if (Ai[TaskNum] >= Sk[MaxDeltaNumber])
				{
					return 1;
				}
				else
				{
					for (int i = 0; i < k; i++)
					{
						if (Ai[TaskNum] > Sk[i])
							CanBeUsed = 1;
					}
					if (!CanBeUsed)
						Ai[TaskNum] = 0;

					return 0;
				}
			}
		} //Функция проверки наличия заданий и обнуления если оставшееся количество не подходит для других групп(для работы)
		void SpendTZ_and_recountDelta(int MaxD)
		{
			float NewDelta;
			int tz_num = chosen_tz[MaxD];
			Ai[tz_num] = Ai[tz_num] - Sk[MaxD];
			X[tz_num][MaxD]++;
			NewDelta = delta_count(delta_mas[MaxD], q[tz_num][MaxD]);
			delta_mas[MaxD] = NewDelta;
		} //Функция распределения заданий 
		void ClearMaxP_ChoseNew_RecountDelta(int MaxD)
		{
			int tz_num;
			float old_delta;
			float old_P;

			tz_num = chosen_tz[MaxD];
			old_P = best_P[MaxD];
			old_delta = delta_mas[MaxD];

			P[tz_num][MaxD] = 0;
			bestP_and_chosen_tz PTz = Find_bestP(MaxD);
			best_P[MaxD] = PTz.bestP;
			chosen_tz[MaxD] = PTz.chosen_tz;

			delta_mas[MaxD] = delta_count(old_delta, old_P, best_P[MaxD]);
		} //Функция очистки текущего максимального Р, выбор другого Р, пересчет дельты
		bool Tasks_have_ended(void)
		{
			bool isEnd = 0;
			int i;
			for (i = 0; i < tz_num; i++)
				if (Ai[i] != 0)
					isEnd = 1;

			return isEnd;
		} //Функция проверки наличия заданий
		float ControlDeep(void)
		{
			float U=0;
			int i;
			int j;
			for (i = 0; i < k; i++)
			{
				float POW = 1;
				for (j = 0; j < tz_num; j++)
				{
					//cout << q[j][i] << " pow " << X[j][i] << "\n";
					POW =  POW * System::Math::Pow(q[j][i], X[j][i]);
				}
				POW = (1 - POW);
				POW = Ck[i] * Sk[i] * POW;
				//cout << "POW = " << POW << "\n";
				U = U + POW;
			}
			return U;
		}//Функция подсчета полноты контроля
		float Effective(float U)
		{
			float E = 0;
			int i;
			for (i = 0; i < k; i++)
				E += (Ck[i] * Sk[i]);
			E = (U / E);

			return E;
		}//Функция подсчета эффективности

		Form1(void)
		{
			InitializeComponent();
			Data_Enter_step1();
			Sk = new int[k];		//Создаем массивы для количества в группах УЭ
			Ck = new int[k];	//и значений их важности
			Ai = new int[tz_num]; // Создаем массив для числа элементов заданий каждого вида
			P = new float *[tz_num]; //Создаем массив для значенйи вероятностей
			q = new float *[tz_num]; //Создаем массив для значений q
			X = new int *[tz_num]; //Создаем массив для значений X
			best_P = new float[k]; //Создаем массив для лучших значений P для каждой из групп
			chosen_tz = new int [k]; //Содаем массив для хранения номеров выбранных заданий
			delta_mas = new float[k]; //Создаем массив для дельт
		}


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox3;

	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::TextBox^  textBox22;
	private: System::Windows::Forms::TextBox^  textBox23;
	private: System::Windows::Forms::TextBox^  textBox24;
	private: System::Windows::Forms::TextBox^  textBox19;
	private: System::Windows::Forms::TextBox^  textBox20;
	private: System::Windows::Forms::TextBox^  textBox21;
	private: System::Windows::Forms::TextBox^  textBox16;
	private: System::Windows::Forms::TextBox^  textBox17;
	private: System::Windows::Forms::TextBox^  textBox18;
	private: System::Windows::Forms::TextBox^  textBox13;
	private: System::Windows::Forms::TextBox^  textBox14;
	private: System::Windows::Forms::TextBox^  textBox15;
	private: System::Windows::Forms::TextBox^  textBox10;
	private: System::Windows::Forms::TextBox^  textBox11;
	private: System::Windows::Forms::TextBox^  textBox12;
	private: System::Windows::Forms::TextBox^  textBox7;
	private: System::Windows::Forms::TextBox^  textBox8;
	private: System::Windows::Forms::TextBox^  textBox9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::Label^  label223;

	private: System::Windows::Forms::Label^  label221;
	private: System::Windows::Forms::Label^  label222;


	private: System::Windows::Forms::TextBox^  textBox27;


	private: System::Windows::Forms::TextBox^  textBox26;
	private: System::Windows::Forms::Label^  label25;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::TextBox^  textBox25;

	private: System::Windows::Forms::Label^  label27;
	private: System::Windows::Forms::TextBox^  textBox30;

	private: System::Windows::Forms::TextBox^  textBox29;
	private: System::Windows::Forms::TextBox^  textBox28;


	private: System::Windows::Forms::Label^  label28;
	private: System::Windows::Forms::Label^  label29;
	private: System::Windows::Forms::Label^  label30;
	private: System::Windows::Forms::Label^  label31;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Button^  button1;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label223 = (gcnew System::Windows::Forms::Label());
			this->label221 = (gcnew System::Windows::Forms::Label());
			this->label222 = (gcnew System::Windows::Forms::Label());
			this->textBox22 = (gcnew System::Windows::Forms::TextBox());
			this->textBox23 = (gcnew System::Windows::Forms::TextBox());
			this->textBox24 = (gcnew System::Windows::Forms::TextBox());
			this->textBox19 = (gcnew System::Windows::Forms::TextBox());
			this->textBox20 = (gcnew System::Windows::Forms::TextBox());
			this->textBox21 = (gcnew System::Windows::Forms::TextBox());
			this->textBox16 = (gcnew System::Windows::Forms::TextBox());
			this->textBox17 = (gcnew System::Windows::Forms::TextBox());
			this->textBox18 = (gcnew System::Windows::Forms::TextBox());
			this->textBox13 = (gcnew System::Windows::Forms::TextBox());
			this->textBox14 = (gcnew System::Windows::Forms::TextBox());
			this->textBox15 = (gcnew System::Windows::Forms::TextBox());
			this->textBox10 = (gcnew System::Windows::Forms::TextBox());
			this->textBox11 = (gcnew System::Windows::Forms::TextBox());
			this->textBox12 = (gcnew System::Windows::Forms::TextBox());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->textBox27 = (gcnew System::Windows::Forms::TextBox());
			this->textBox26 = (gcnew System::Windows::Forms::TextBox());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->textBox25 = (gcnew System::Windows::Forms::TextBox());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->textBox30 = (gcnew System::Windows::Forms::TextBox());
			this->textBox29 = (gcnew System::Windows::Forms::TextBox());
			this->textBox28 = (gcnew System::Windows::Forms::TextBox());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(36, 91);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(20, 20);
			this->label1->TabIndex = 0;
			this->label1->Text = L"K";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label2->Location = System::Drawing::Point(36, 118);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(13, 20);
			this->label2->TabIndex = 1;
			this->label2->Text = L"j";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label3->Location = System::Drawing::Point(33, 153);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(34, 20);
			this->label3->TabIndex = 2;
			this->label3->Text = L"Ski";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label4->Location = System::Drawing::Point(33, 188);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(25, 20);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Cj";
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox1->Location = System::Drawing::Point(14, 72);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(45, 22);
			this->textBox1->TabIndex = 1;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Form1::textBox1_TextChanged);
			this->textBox1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox1_KeyPress);
			// 
			// textBox2
			// 
			this->textBox2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox2->Location = System::Drawing::Point(65, 72);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(45, 22);
			this->textBox2->TabIndex = 2;
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &Form1::textBox2_TextChanged);
			this->textBox2->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox2_KeyPress);
			// 
			// textBox3
			// 
			this->textBox3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox3->Location = System::Drawing::Point(116, 72);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(45, 22);
			this->textBox3->TabIndex = 3;
			this->textBox3->TextChanged += gcnew System::EventHandler(this, &Form1::textBox3_TextChanged);
			this->textBox3->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox3_KeyPress);
			// 
			// textBox5
			// 
			this->textBox5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox5->Location = System::Drawing::Point(65, 107);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(45, 22);
			this->textBox5->TabIndex = 5;
			this->textBox5->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox5_KeyPress);
			// 
			// textBox6
			// 
			this->textBox6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox6->Location = System::Drawing::Point(116, 107);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(45, 22);
			this->textBox6->TabIndex = 6;
			this->textBox6->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox6_KeyPress);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label5->Location = System::Drawing::Point(25, 11);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(19, 20);
			this->label5->TabIndex = 10;
			this->label5->Text = L"1";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label6->Location = System::Drawing::Point(76, 11);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(19, 20);
			this->label6->TabIndex = 11;
			this->label6->Text = L"2";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label7->Location = System::Drawing::Point(127, 11);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(19, 20);
			this->label7->TabIndex = 12;
			this->label7->Text = L"3";
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::AliceBlue;
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel1->Controls->Add(this->label223);
			this->panel1->Controls->Add(this->label221);
			this->panel1->Controls->Add(this->label222);
			this->panel1->Controls->Add(this->textBox22);
			this->panel1->Controls->Add(this->textBox23);
			this->panel1->Controls->Add(this->textBox24);
			this->panel1->Controls->Add(this->textBox19);
			this->panel1->Controls->Add(this->textBox20);
			this->panel1->Controls->Add(this->textBox21);
			this->panel1->Controls->Add(this->textBox16);
			this->panel1->Controls->Add(this->textBox17);
			this->panel1->Controls->Add(this->textBox18);
			this->panel1->Controls->Add(this->textBox13);
			this->panel1->Controls->Add(this->textBox14);
			this->panel1->Controls->Add(this->textBox15);
			this->panel1->Controls->Add(this->textBox10);
			this->panel1->Controls->Add(this->textBox11);
			this->panel1->Controls->Add(this->textBox12);
			this->panel1->Controls->Add(this->textBox7);
			this->panel1->Controls->Add(this->textBox8);
			this->panel1->Controls->Add(this->textBox9);
			this->panel1->Controls->Add(this->textBox4);
			this->panel1->Controls->Add(this->textBox3);
			this->panel1->Controls->Add(this->textBox2);
			this->panel1->Controls->Add(this->textBox5);
			this->panel1->Controls->Add(this->label7);
			this->panel1->Controls->Add(this->label5);
			this->panel1->Controls->Add(this->textBox6);
			this->panel1->Controls->Add(this->textBox1);
			this->panel1->Controls->Add(this->label6);
			this->panel1->Location = System::Drawing::Point(84, 79);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(179, 348);
			this->panel1->TabIndex = 13;
			// 
			// label223
			// 
			this->label223->AutoSize = true;
			this->label223->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label223->Location = System::Drawing::Point(117, 39);
			this->label223->Name = L"label223";
			this->label223->Size = System::Drawing::Size(39, 20);
			this->label223->TabIndex = 33;
			this->label223->Text = L"_÷_";
			// 
			// label221
			// 
			this->label221->AutoSize = true;
			this->label221->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label221->Location = System::Drawing::Point(14, 39);
			this->label221->Name = L"label221";
			this->label221->Size = System::Drawing::Size(39, 20);
			this->label221->TabIndex = 31;
			this->label221->Text = L"_÷_";
			// 
			// label222
			// 
			this->label222->AutoSize = true;
			this->label222->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label222->Location = System::Drawing::Point(66, 39);
			this->label222->Name = L"label222";
			this->label222->Size = System::Drawing::Size(39, 20);
			this->label222->TabIndex = 32;
			this->label222->Text = L"_÷_";
			// 
			// textBox22
			// 
			this->textBox22->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox22->Location = System::Drawing::Point(14, 307);
			this->textBox22->Name = L"textBox22";
			this->textBox22->Size = System::Drawing::Size(45, 22);
			this->textBox22->TabIndex = 28;
			this->textBox22->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox22_KeyPress);
			// 
			// textBox23
			// 
			this->textBox23->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox23->Location = System::Drawing::Point(65, 307);
			this->textBox23->Name = L"textBox23";
			this->textBox23->Size = System::Drawing::Size(45, 22);
			this->textBox23->TabIndex = 29;
			this->textBox23->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox23_KeyPress);
			// 
			// textBox24
			// 
			this->textBox24->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox24->Location = System::Drawing::Point(116, 307);
			this->textBox24->Name = L"textBox24";
			this->textBox24->Size = System::Drawing::Size(45, 22);
			this->textBox24->TabIndex = 30;
			this->textBox24->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox24_KeyPress);
			// 
			// textBox19
			// 
			this->textBox19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox19->Location = System::Drawing::Point(14, 274);
			this->textBox19->Name = L"textBox19";
			this->textBox19->Size = System::Drawing::Size(45, 22);
			this->textBox19->TabIndex = 25;
			this->textBox19->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox19_KeyPress);
			// 
			// textBox20
			// 
			this->textBox20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox20->Location = System::Drawing::Point(65, 274);
			this->textBox20->Name = L"textBox20";
			this->textBox20->Size = System::Drawing::Size(45, 22);
			this->textBox20->TabIndex = 26;
			this->textBox20->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox20_KeyPress);
			// 
			// textBox21
			// 
			this->textBox21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox21->Location = System::Drawing::Point(116, 274);
			this->textBox21->Name = L"textBox21";
			this->textBox21->Size = System::Drawing::Size(45, 22);
			this->textBox21->TabIndex = 27;
			this->textBox21->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox21_KeyPress);
			// 
			// textBox16
			// 
			this->textBox16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox16->Location = System::Drawing::Point(14, 242);
			this->textBox16->Name = L"textBox16";
			this->textBox16->Size = System::Drawing::Size(45, 22);
			this->textBox16->TabIndex = 22;
			this->textBox16->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox16_KeyPress);
			// 
			// textBox17
			// 
			this->textBox17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox17->Location = System::Drawing::Point(65, 242);
			this->textBox17->Name = L"textBox17";
			this->textBox17->Size = System::Drawing::Size(45, 22);
			this->textBox17->TabIndex = 23;
			this->textBox17->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox17_KeyPress);
			// 
			// textBox18
			// 
			this->textBox18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox18->Location = System::Drawing::Point(116, 242);
			this->textBox18->Name = L"textBox18";
			this->textBox18->Size = System::Drawing::Size(45, 22);
			this->textBox18->TabIndex = 24;
			this->textBox18->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox18_KeyPress);
			// 
			// textBox13
			// 
			this->textBox13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox13->Location = System::Drawing::Point(14, 209);
			this->textBox13->Name = L"textBox13";
			this->textBox13->Size = System::Drawing::Size(45, 22);
			this->textBox13->TabIndex = 19;
			this->textBox13->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox13_KeyPress);
			// 
			// textBox14
			// 
			this->textBox14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox14->Location = System::Drawing::Point(65, 209);
			this->textBox14->Name = L"textBox14";
			this->textBox14->Size = System::Drawing::Size(45, 22);
			this->textBox14->TabIndex = 20;
			this->textBox14->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox14_KeyPress);
			// 
			// textBox15
			// 
			this->textBox15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox15->Location = System::Drawing::Point(116, 209);
			this->textBox15->Name = L"textBox15";
			this->textBox15->Size = System::Drawing::Size(45, 22);
			this->textBox15->TabIndex = 21;
			this->textBox15->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox15_KeyPress);
			// 
			// textBox10
			// 
			this->textBox10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox10->Location = System::Drawing::Point(14, 177);
			this->textBox10->Name = L"textBox10";
			this->textBox10->Size = System::Drawing::Size(45, 22);
			this->textBox10->TabIndex = 16;
			this->textBox10->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox10_KeyPress);
			// 
			// textBox11
			// 
			this->textBox11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox11->Location = System::Drawing::Point(65, 177);
			this->textBox11->Name = L"textBox11";
			this->textBox11->Size = System::Drawing::Size(45, 22);
			this->textBox11->TabIndex = 17;
			this->textBox11->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox11_KeyPress);
			// 
			// textBox12
			// 
			this->textBox12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox12->Location = System::Drawing::Point(116, 177);
			this->textBox12->Name = L"textBox12";
			this->textBox12->Size = System::Drawing::Size(45, 22);
			this->textBox12->TabIndex = 18;
			this->textBox12->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox12_KeyPress);
			// 
			// textBox7
			// 
			this->textBox7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox7->Location = System::Drawing::Point(14, 142);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(45, 22);
			this->textBox7->TabIndex = 13;
			this->textBox7->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox7_KeyPress);
			// 
			// textBox8
			// 
			this->textBox8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox8->Location = System::Drawing::Point(65, 142);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(45, 22);
			this->textBox8->TabIndex = 14;
			this->textBox8->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox8_KeyPress);
			// 
			// textBox9
			// 
			this->textBox9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox9->Location = System::Drawing::Point(116, 142);
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(45, 22);
			this->textBox9->TabIndex = 15;
			this->textBox9->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox9_KeyPress);
			// 
			// textBox4
			// 
			this->textBox4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox4->Location = System::Drawing::Point(14, 107);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(45, 22);
			this->textBox4->TabIndex = 4;
			this->textBox4->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::textBox4_KeyPress);
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Underline, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label8->Location = System::Drawing::Point(10, 9);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(291, 25);
			this->label8->TabIndex = 14;
			this->label8->Text = L"Заполните входные данные";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label9->Location = System::Drawing::Point(38, 49);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(157, 20);
			this->label9->TabIndex = 15;
			this->label9->Text = L"Количество тестов";
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->numericUpDown1->Location = System::Drawing::Point(217, 47);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {6, 0, 0, 0});
			this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {2, 0, 0, 0});
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(46, 26);
			this->numericUpDown1->TabIndex = 0;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {6, 0, 0, 0});
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &Form1::numericUpDown1_ValueChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label10->Location = System::Drawing::Point(33, 223);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(20, 20);
			this->label10->TabIndex = 17;
			this->label10->Text = L"P";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label11->Location = System::Drawing::Point(49, 227);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(18, 16);
			this->label11->TabIndex = 18;
			this->label11->Text = L"1j";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label12->Location = System::Drawing::Point(49, 260);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(18, 16);
			this->label12->TabIndex = 20;
			this->label12->Text = L"2j";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label13->Location = System::Drawing::Point(33, 254);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(20, 20);
			this->label13->TabIndex = 19;
			this->label13->Text = L"P";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label14->Location = System::Drawing::Point(49, 292);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(18, 16);
			this->label14->TabIndex = 22;
			this->label14->Text = L"3j";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label15->Location = System::Drawing::Point(33, 286);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(20, 20);
			this->label15->TabIndex = 21;
			this->label15->Text = L"P";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label16->Location = System::Drawing::Point(49, 325);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(18, 16);
			this->label16->TabIndex = 24;
			this->label16->Text = L"4j";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label17->Location = System::Drawing::Point(33, 319);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(20, 20);
			this->label17->TabIndex = 23;
			this->label17->Text = L"P";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label18->Location = System::Drawing::Point(49, 357);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(18, 16);
			this->label18->TabIndex = 26;
			this->label18->Text = L"5j";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label19->Location = System::Drawing::Point(33, 351);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(20, 20);
			this->label19->TabIndex = 25;
			this->label19->Text = L"P";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label20->Location = System::Drawing::Point(49, 390);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(18, 16);
			this->label20->TabIndex = 28;
			this->label20->Text = L"6j";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label21->Location = System::Drawing::Point(33, 384);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(20, 20);
			this->label21->TabIndex = 27;
			this->label21->Text = L"P";
			// 
			// textBox27
			// 
			this->textBox27->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox27->Location = System::Drawing::Point(120, 515);
			this->textBox27->Name = L"textBox27";
			this->textBox27->Size = System::Drawing::Size(38, 22);
			this->textBox27->TabIndex = 33;
			// 
			// textBox26
			// 
			this->textBox26->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox26->Location = System::Drawing::Point(74, 515);
			this->textBox26->Name = L"textBox26";
			this->textBox26->Size = System::Drawing::Size(38, 22);
			this->textBox26->TabIndex = 32;
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label25->Location = System::Drawing::Point(132, 487);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(18, 20);
			this->label25->TabIndex = 34;
			this->label25->Text = L"3";
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label26->Location = System::Drawing::Point(40, 487);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(18, 20);
			this->label26->TabIndex = 32;
			this->label26->Text = L"1";
			// 
			// textBox25
			// 
			this->textBox25->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox25->Location = System::Drawing::Point(28, 515);
			this->textBox25->Name = L"textBox25";
			this->textBox25->Size = System::Drawing::Size(38, 22);
			this->textBox25->TabIndex = 31;
			// 
			// label27
			// 
			this->label27->AutoSize = true;
			this->label27->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label27->Location = System::Drawing::Point(86, 487);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(18, 20);
			this->label27->TabIndex = 33;
			this->label27->Text = L"2";
			// 
			// textBox30
			// 
			this->textBox30->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox30->Location = System::Drawing::Point(258, 515);
			this->textBox30->Name = L"textBox30";
			this->textBox30->Size = System::Drawing::Size(38, 22);
			this->textBox30->TabIndex = 36;
			this->textBox30->TextChanged += gcnew System::EventHandler(this, &Form1::textBox28_TextChanged);
			// 
			// textBox29
			// 
			this->textBox29->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox29->Location = System::Drawing::Point(212, 515);
			this->textBox29->Name = L"textBox29";
			this->textBox29->Size = System::Drawing::Size(38, 22);
			this->textBox29->TabIndex = 35;
			// 
			// textBox28
			// 
			this->textBox28->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->textBox28->Location = System::Drawing::Point(166, 515);
			this->textBox28->Name = L"textBox28";
			this->textBox28->Size = System::Drawing::Size(38, 22);
			this->textBox28->TabIndex = 34;
			// 
			// label28
			// 
			this->label28->AutoSize = true;
			this->label28->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label28->Location = System::Drawing::Point(269, 487);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(18, 20);
			this->label28->TabIndex = 40;
			this->label28->Text = L"6";
			// 
			// label29
			// 
			this->label29->AutoSize = true;
			this->label29->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label29->Location = System::Drawing::Point(177, 487);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(18, 20);
			this->label29->TabIndex = 38;
			this->label29->Text = L"4";
			// 
			// label30
			// 
			this->label30->AutoSize = true;
			this->label30->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label30->Location = System::Drawing::Point(223, 487);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(18, 20);
			this->label30->TabIndex = 39;
			this->label30->Text = L"5";
			// 
			// label31
			// 
			this->label31->AutoSize = true;
			this->label31->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label31->Location = System::Drawing::Point(18, 440);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(236, 20);
			this->label31->TabIndex = 41;
			this->label31->Text = L"Количество заданий в тестах";
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::AliceBlue;
			this->panel2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->panel2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->panel2->Location = System::Drawing::Point(15, 477);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(295, 72);
			this->panel2->TabIndex = 34;
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->button1->Location = System::Drawing::Point(12, 566);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(298, 29);
			this->button1->TabIndex = 42;
			this->button1->Text = L"Рассчитать";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::AliceBlue;
			this->ClientSize = System::Drawing::Size(327, 612);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label31);
			this->Controls->Add(this->label28);
			this->Controls->Add(this->label29);
			this->Controls->Add(this->label30);
			this->Controls->Add(this->textBox30);
			this->Controls->Add(this->textBox29);
			this->Controls->Add(this->textBox28);
			this->Controls->Add(this->textBox27);
			this->Controls->Add(this->textBox26);
			this->Controls->Add(this->label25);
			this->Controls->Add(this->label26);
			this->Controls->Add(this->textBox25);
			this->Controls->Add(this->label27);
			this->Controls->Add(this->label20);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->panel2);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Name = L"Form1";
			this->Text = L"Рассчет эффективности";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numericUpDown1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void Form1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
			 {
				 if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08)
					 e->Handled = true;
			 }
	private: System::Void textBox1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08)
				 e->Handled = true;
			 }
	private: System::Void textBox2_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08)
				 e->Handled = true;
			 }
	private: System::Void textBox3_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08)
				 e->Handled = true;
			 }
	private: System::Void textBox4_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08)
				 e->Handled = true;
			 }
	private: System::Void textBox5_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08)
				 e->Handled = true;
			 }
	private: System::Void textBox6_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08)
				 e->Handled = true;
			 }
	private: System::Void textBox7_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox8_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox9_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox10_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox11_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox12_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox13_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox14_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox15_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox16_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox17_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox18_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox19_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox20_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox21_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox22_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox23_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void textBox24_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {if(!Char::IsDigit(e->KeyChar)&& e->KeyChar != 0x08 && e->KeyChar != 0x2C)
				 e->Handled = true;
			 }
	private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Data_Enter_step1();


				 switch (numericUpDown1->Value.ToInt32(numericUpDown1->Value))
				 {
				 case 2:
					 {					 
						 textBox13->Enabled = false;
						 textBox14->Enabled = false;
						 textBox15->Enabled = false;
						 textBox16->Enabled = false;
						 textBox17->Enabled = false;
						 textBox18->Enabled = false;
						 textBox19->Enabled = false;
						 textBox20->Enabled = false;
						 textBox21->Enabled = false;
						 textBox22->Enabled = false;
						 textBox23->Enabled = false;
						 textBox24->Enabled = false;

						 textBox27->Enabled = false;
						 textBox28->Enabled = false;
						 textBox29->Enabled = false;
						 textBox30->Enabled = false;
						 break;
					 }
				 case 3:
					 {					 
						 textBox13->Enabled = true;
						 textBox14->Enabled = true;
						 textBox15->Enabled = true;
						 textBox16->Enabled = false;
						 textBox17->Enabled = false;
						 textBox18->Enabled = false;
						 textBox19->Enabled = false;
						 textBox20->Enabled = false;
						 textBox21->Enabled = false;
						 textBox22->Enabled = false;
						 textBox23->Enabled = false;
						 textBox24->Enabled = false;

						 textBox27->Enabled = true;
						 textBox28->Enabled = false;
						 textBox29->Enabled = false;
						 textBox30->Enabled = false;
						 break;
					 }
				 case 4:
					 {					 
						 textBox13->Enabled = true;
						 textBox14->Enabled = true;
						 textBox15->Enabled = true;
						 textBox16->Enabled = true;
						 textBox17->Enabled = true;
						 textBox18->Enabled = true;
						 textBox19->Enabled = false;
						 textBox20->Enabled = false;
						 textBox21->Enabled = false;
						 textBox22->Enabled = false;
						 textBox23->Enabled = false;
						 textBox24->Enabled = false;

						 textBox27->Enabled = true;
						 textBox28->Enabled = true;
						 textBox29->Enabled = false;
						 textBox30->Enabled = false;
						 break;
					 }
				 case 5:
					 {					 
						 textBox13->Enabled = true;
						 textBox14->Enabled = true;
						 textBox15->Enabled = true;
						 textBox16->Enabled = true;
						 textBox17->Enabled = true;
						 textBox18->Enabled = true;
						 textBox19->Enabled = true;
						 textBox20->Enabled = true;
						 textBox21->Enabled = true;
						 textBox22->Enabled = false;
						 textBox23->Enabled = false;
						 textBox24->Enabled = false;

						 textBox27->Enabled = true;
						 textBox28->Enabled = true;
						 textBox29->Enabled = true;
						 textBox30->Enabled = false;
						 break;
					 }
				 case 6:
					 {					 
						 textBox13->Enabled = true;
						 textBox14->Enabled = true;
						 textBox15->Enabled = true;
						 textBox16->Enabled = true;
						 textBox17->Enabled = true;
						 textBox18->Enabled = true;
						 textBox19->Enabled = true;
						 textBox20->Enabled = true;
						 textBox21->Enabled = true;
						 textBox22->Enabled = true;
						 textBox23->Enabled = true;
						 textBox24->Enabled = true;

						 textBox27->Enabled = true;
						 textBox28->Enabled = true;
						 textBox29->Enabled = true;
						 textBox30->Enabled = true;
						 break;
					 }
				 }
			 }
	private: System::Void textBox28_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }

	private: void GetInputs()
			 {
				 Data_Enter_step2();
				 Data_Enter_step3(); // Заполняем P
				 Fill_bestP_and_chosentz(); //Создаем начальный массив лучших P и запоминаем номера выбранных заданий для каждой группы k
				 First_delta_count(); //Подсчитываем начальные дельты
			 }
	private: void GetMath()
			 {
				 int MaxDeltaNum;
				 bool isNotEnd = 1;
				 bool isTasks;
				 int i = 1;
				 while (isNotEnd)
				 {
					 MaxDeltaNum = MaxDelta();		
					 isTasks = IsTasksThere(MaxDeltaNum);
					 if (isTasks)
					 {
						 SpendTZ_and_recountDelta(MaxDeltaNum);
					 }		
					 else
					 {
						 ClearMaxP_ChoseNew_RecountDelta(MaxDeltaNum);
					 }		
					 i++;
					 isNotEnd = Tasks_have_ended();
				 }

				 U = ControlDeep();
				 E = Effective(U);

			 }

	private: String^ GetX()
			 {
				 String^ s = "Количество заданий:";
				 for(int i = 0; i<k; i++)
				 {
					 s += "\n Модуль " + (i+1) + ": ";
					 for(int j = 0; j<numericUpDown1->Value.ToInt32(numericUpDown1->Value);j++)
					 {
						 s += X[j][i] + " ";
					 }
				 }
				 return s;
			 }

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if(textBox1->Text!="" &&
					 textBox2->Text!="" &&
					 textBox3->Text!="" &&
					 textBox4->Text!="" &&
					 textBox5->Text!="" &&
					 textBox6->Text!="" &&
					 textBox7->Text!="" &&
					 textBox8->Text!="" &&
					 textBox9->Text!="" &&
					 textBox10->Text!="" &&
					 textBox11->Text!="" &&
					 textBox12->Text!="" &&
					 textBox25->Text!="" &&
					 textBox26->Text!="" 		 				 
					 )
				 {
					 switch (numericUpDown1->Value.ToInt32(numericUpDown1->Value))
					 {
					 case 2:
						 {				 

							 GetInputs();
							 GetMath();

							 MessageBox::Show(GetX() + "\nПолнота контроля: Y = " + U + "\nЭффективность: E = " + E);
							 break;
						 }
					 case 3:
						 {	
							 if(textBox13->Text!="" &&
								 textBox14->Text!="" &&
								 textBox15->Text!="" &&
								 textBox27->Text!="" 	

								 )
							 {

								 GetInputs();
								 GetMath();

								 MessageBox::Show(GetX() + "\nПолнота контроля: Y = " + U + "\nЭффективность: E = " + E);
							 }
							 else
							 {
								 MessageBox::Show("Заполните все поля ввода");
							 }
							 break;
						 }
					 case 4:
						 {	
							 if(textBox13->Text!="" &&
								 textBox14->Text!="" &&
								 textBox15->Text!="" && 
								 textBox16->Text!="" &&
								 textBox17->Text!="" &&
								 textBox18->Text!="" &&
								 textBox28->Text!="" 	
								 )
							 {

								 GetInputs();
								 GetMath();

								 MessageBox::Show(GetX() + "\nПолнота контроля: Y = " + U + "\nЭффективность: E = " + E);
							 }
							 else
							 {
								 MessageBox::Show("Заполните все поля ввода");
							 }
							 break;
						 }
					 case 5:
						 {	
							 if(textBox13->Text!="" &&
								 textBox14->Text!="" &&
								 textBox15->Text!="" && 
								 textBox16->Text!="" &&
								 textBox17->Text!="" &&
								 textBox18->Text!="" && 
								 textBox19->Text!="" &&
								 textBox20->Text!="" &&
								 textBox21->Text!="" &&
								 textBox29->Text!="" 	
								 )
							 {

								 GetInputs();
								 GetMath();

								 MessageBox::Show(GetX() + "\nПолнота контроля: Y = " + U + "\nЭффективность: E = " + E);
							 }
							 else
							 {
								 MessageBox::Show("Заполните все поля ввода");
							 }
							 break;
						 }
					 case 6:
						 {	
							  if(textBox13->Text!="" &&
								 textBox14->Text!="" &&
								 textBox15->Text!="" && 
								 textBox16->Text!="" &&
								 textBox17->Text!="" &&
								 textBox18->Text!="" && 
								 textBox19->Text!="" &&
								 textBox20->Text!="" &&
								 textBox21->Text!="" &&
								 textBox22->Text!="" && 
								 textBox23->Text!="" &&
								 textBox24->Text!="" &&
								 textBox30->Text!="" 	
								 )
							 {

								 GetInputs();
								 GetMath();

								 MessageBox::Show(GetX() + "\nПолнота контроля: Y = " + U + "\nЭффективность: E = " + E);
							 }
							 else
							 {
								 MessageBox::Show("Заполните все поля ввода");
							 }
							 break;
						 }


					 }
				 }
				 else
				 {
					 MessageBox::Show("Заполните все поля ввода");
				 }
			 }

	private: void Count()
			 {
				 if(textBox1->Text!="")
				 {
					 int c = 1;
					 int c1 = c+System::Convert::ToInt32(textBox1->Text)-1;
					 label221->Text = c.ToString() + "-" + c1.ToString();
				 }			 
				 if(textBox1->Text!="" && textBox2->Text!=""&& textBox3->Text!="")
				 {
					 int c = 1;
					 int c1 = c+System::Convert::ToInt32(textBox1->Text);
					 c = c1;
					 c1 = c+System::Convert::ToInt32(textBox2->Text);
					 c = c1;
					 c1 = c+System::Convert::ToInt32(textBox3->Text)-1;
					 label223->Text = c.ToString() + "-" + c1.ToString();
				 }
				 if(textBox1->Text!=""&& textBox2->Text!="")
				 {
					 int c = 1;
					 int c1 = c+System::Convert::ToInt32(textBox1->Text);
					 c = c1;
					 c1 = c+System::Convert::ToInt32(textBox2->Text)-1;
					 label222->Text = c.ToString() + "-" + c1.ToString();
				 }

			 }


	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Count();
			 }
	private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Count();
			 }
	private: System::Void textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Count();
			 }
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
};
}

