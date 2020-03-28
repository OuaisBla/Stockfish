#pragma once

#include <msclr\marshal_cppstd.h>
#include <cliext/vector> 
#include <math.h>
#include "EvalStacker.h"
#include "PGN.h"
#include "TrendDetector.h"


namespace EvalViewer {

  using namespace System;
  using namespace System::ComponentModel;
  using namespace System::Collections;
  using namespace System::Collections::Generic;
  using namespace System::Windows::Forms;
  using namespace System::Data;
  using namespace System::Text;
  using namespace System::Drawing;

  /// <summary>
  /// Summary for Form1
  /// </summary>
  public ref class EvalGraph : public System::Windows::Forms::Form
  {
  public:
    EvalGraph(void)
    {
      InitializeComponent();
      //
      //TODO: Add the constructor code here
      //
    }

  protected:
    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    ~EvalGraph()
    {
      delete m_pTheMyEval;
      delete p_theMyTournament;

      if (components)
      {
        delete components;
      }
    }


    PGNTournament *p_theMyTournament = nullptr;

    EvalStacker *m_pTheMyEval = nullptr;

  private: System::Windows::Forms::DataVisualization::Charting::Series^  seriesBase;
  private: System::Windows::Forms::DataVisualization::Charting::Series^  seriesPoly;
  private: System::Windows::Forms::DataVisualization::Charting::Series^  seriesLog;
  private: System::Windows::Forms::DataVisualization::Charting::Series^  seriesExp;
  private: System::Windows::Forms::DataVisualization::Charting::Series^  seriesPow;
  private: System::Windows::Forms::DataVisualization::Charting::Chart^  polyfitChart;
  private: System::Windows::Forms::ComboBox^  comboBoxSide;
  private: System::Windows::Forms::ComboBox^  comboBoxNames;
  private: System::Windows::Forms::ContextMenuStrip^  contextMenuStripFitLines;
  private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItemPolynomialFit;
  private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItemExpFit;
  private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItemLogFit;
  private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItemPowFit;
  private: System::Windows::Forms::CheckBox^  checkBoxReverse;

  private: System::ComponentModel::IContainer^  components;





  protected:

  private:
    /// <summary>
    /// Required designer variable.
    /// </summary>


#pragma region Windows Form Designer generated code
    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    void InitializeComponent(void)
    {
      this->components = (gcnew System::ComponentModel::Container());
      System::Windows::Forms::Button^  buttonGraph;
      System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
      System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
      System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
      System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
      System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
      System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
      System::Windows::Forms::DataVisualization::Charting::Series^  series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
      System::Windows::Forms::DataVisualization::Charting::Series^  series5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
      this->polyfitChart = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
      this->contextMenuStripFitLines = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
      this->toolStripMenuItemPolynomialFit = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->toolStripMenuItemExpFit = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->toolStripMenuItemLogFit = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->toolStripMenuItemPowFit = (gcnew System::Windows::Forms::ToolStripMenuItem());
      this->comboBoxSide = (gcnew System::Windows::Forms::ComboBox());
      this->comboBoxNames = (gcnew System::Windows::Forms::ComboBox());
      this->checkBoxReverse = (gcnew System::Windows::Forms::CheckBox());
      buttonGraph = (gcnew System::Windows::Forms::Button());
      tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
      tableLayoutPanel1->SuspendLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->polyfitChart))->BeginInit();
      this->contextMenuStripFitLines->SuspendLayout();
      this->SuspendLayout();
      // 
      // buttonGraph
      // 
      buttonGraph->Dock = System::Windows::Forms::DockStyle::Fill;
      buttonGraph->Location = System::Drawing::Point(685, 454);
      buttonGraph->Margin = System::Windows::Forms::Padding(4);
      buttonGraph->Name = L"buttonGraph";
      buttonGraph->Size = System::Drawing::Size(114, 30);
      buttonGraph->TabIndex = 1;
      buttonGraph->Text = L"&Graph";
      buttonGraph->UseVisualStyleBackColor = true;
      buttonGraph->Click += gcnew System::EventHandler(this, &EvalGraph::buttonGraph_Click);
      // 
      // tableLayoutPanel1
      // 
      tableLayoutPanel1->ColumnCount = 4;
      tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 23.46491F)));
      tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 146)));
      tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 76.53509F)));
      tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute, 121)));
      tableLayoutPanel1->Controls->Add(this->polyfitChart, 0, 0);
      tableLayoutPanel1->Controls->Add(buttonGraph, 3, 1);
      tableLayoutPanel1->Controls->Add(this->comboBoxSide, 0, 1);
      tableLayoutPanel1->Controls->Add(this->comboBoxNames, 2, 1);
      tableLayoutPanel1->Controls->Add(this->checkBoxReverse, 1, 1);
      tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
      tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
      tableLayoutPanel1->Margin = System::Windows::Forms::Padding(4);
      tableLayoutPanel1->Name = L"tableLayoutPanel1";
      tableLayoutPanel1->RowCount = 2;
      tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
      tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 38)));
      tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 25)));
      tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 25)));
      tableLayoutPanel1->Size = System::Drawing::Size(803, 488);
      tableLayoutPanel1->TabIndex = 0;
      // 
      // polyfitChart
      // 
      chartArea1->Name = L"ChartArea1";
      this->polyfitChart->ChartAreas->Add(chartArea1);
      tableLayoutPanel1->SetColumnSpan(this->polyfitChart, 4);
      this->polyfitChart->ContextMenuStrip = this->contextMenuStripFitLines;
      this->polyfitChart->Dock = System::Windows::Forms::DockStyle::Fill;
      legend1->Name = L"Legend1";
      this->polyfitChart->Legends->Add(legend1);
      this->polyfitChart->Location = System::Drawing::Point(4, 4);
      this->polyfitChart->Margin = System::Windows::Forms::Padding(4);
      this->polyfitChart->Name = L"polyfitChart";
      series1->ChartArea = L"ChartArea1";
      series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Point;
      series1->Legend = L"Legend1";
      series1->Name = L"Data1";
      series2->BorderWidth = 3;
      series2->ChartArea = L"ChartArea1";
      series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
      series2->Color = System::Drawing::Color::Red;
      series2->Legend = L"Legend1";
      series2->Name = L"PolyFit1";
      series3->BorderWidth = 3;
      series3->ChartArea = L"ChartArea1";
      series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
      series3->Color = System::Drawing::Color::Green;
      series3->Legend = L"Legend1";
      series3->Name = L"ExpFit1";
      series4->BorderWidth = 3;
      series4->ChartArea = L"ChartArea1";
      series4->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
      series4->Color = System::Drawing::Color::MediumBlue;
      series4->Legend = L"Legend1";
      series4->Name = L"LogFit1";
      series5->ChartArea = L"ChartArea1";
      series5->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
      series5->Legend = L"Legend1";
      series5->Name = L"PowFit1";
      this->polyfitChart->Series->Add(series1);
      this->polyfitChart->Series->Add(series2);
      this->polyfitChart->Series->Add(series3);
      this->polyfitChart->Series->Add(series4);
      this->polyfitChart->Series->Add(series5);
      this->polyfitChart->Size = System::Drawing::Size(795, 442);
      this->polyfitChart->TabIndex = 2;
      this->polyfitChart->Text = L"chart";
      // 
      // contextMenuStripFitLines
      // 
      this->contextMenuStripFitLines->ImageScalingSize = System::Drawing::Size(20, 20);
      this->contextMenuStripFitLines->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
        this->toolStripMenuItemPolynomialFit,
          this->toolStripMenuItemExpFit, this->toolStripMenuItemLogFit, this->toolStripMenuItemPowFit
      });
      this->contextMenuStripFitLines->Name = L"contextMenuStrip1";
      this->contextMenuStripFitLines->Size = System::Drawing::Size(177, 108);
      // 
      // toolStripMenuItemPolynomialFit
      // 
      this->toolStripMenuItemPolynomialFit->Checked = true;
      this->toolStripMenuItemPolynomialFit->CheckOnClick = true;
      this->toolStripMenuItemPolynomialFit->CheckState = System::Windows::Forms::CheckState::Checked;
      this->toolStripMenuItemPolynomialFit->Name = L"toolStripMenuItemPolynomialFit";
      this->toolStripMenuItemPolynomialFit->Size = System::Drawing::Size(176, 26);
      this->toolStripMenuItemPolynomialFit->Text = L"Polynomial Fit";
      // 
      // toolStripMenuItemExpFit
      // 
      this->toolStripMenuItemExpFit->Checked = true;
      this->toolStripMenuItemExpFit->CheckOnClick = true;
      this->toolStripMenuItemExpFit->CheckState = System::Windows::Forms::CheckState::Checked;
      this->toolStripMenuItemExpFit->Name = L"toolStripMenuItemExpFit";
      this->toolStripMenuItemExpFit->Size = System::Drawing::Size(176, 26);
      this->toolStripMenuItemExpFit->Text = L"Exponential Fit";
      // 
      // toolStripMenuItemLogFit
      // 
      this->toolStripMenuItemLogFit->Checked = true;
      this->toolStripMenuItemLogFit->CheckOnClick = true;
      this->toolStripMenuItemLogFit->CheckState = System::Windows::Forms::CheckState::Checked;
      this->toolStripMenuItemLogFit->Name = L"toolStripMenuItemLogFit";
      this->toolStripMenuItemLogFit->Size = System::Drawing::Size(176, 26);
      this->toolStripMenuItemLogFit->Text = L"Logarithm Fit";
      // 
      // toolStripMenuItemPowFit
      // 
      this->toolStripMenuItemPowFit->CheckOnClick = true;
      this->toolStripMenuItemPowFit->Name = L"toolStripMenuItemPowFit";
      this->toolStripMenuItemPowFit->Size = System::Drawing::Size(176, 26);
      this->toolStripMenuItemPowFit->Text = L"Power-Law Fit";
      // 
      // comboBoxSide
      // 
      this->comboBoxSide->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
      this->comboBoxSide->FormattingEnabled = true;
      this->comboBoxSide->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Curve 1", L"Curve 2" });
      this->comboBoxSide->Location = System::Drawing::Point(3, 453);
      this->comboBoxSide->Name = L"comboBoxSide";
      this->comboBoxSide->Size = System::Drawing::Size(119, 24);
      this->comboBoxSide->TabIndex = 3;
      // 
      // comboBoxNames
      // 
      this->comboBoxNames->Dock = System::Windows::Forms::DockStyle::Fill;
      this->comboBoxNames->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
      this->comboBoxNames->FormattingEnabled = true;
      this->comboBoxNames->Location = System::Drawing::Point(274, 453);
      this->comboBoxNames->Name = L"comboBoxNames";
      this->comboBoxNames->Size = System::Drawing::Size(404, 24);
      this->comboBoxNames->TabIndex = 4;
      // 
      // checkBoxReverse
      // 
      this->checkBoxReverse->AutoSize = true;
      this->checkBoxReverse->Checked = true;
      this->checkBoxReverse->CheckState = System::Windows::Forms::CheckState::Checked;
      this->checkBoxReverse->Location = System::Drawing::Point(128, 453);
      this->checkBoxReverse->Name = L"checkBoxReverse";
      this->checkBoxReverse->Size = System::Drawing::Size(116, 21);
      this->checkBoxReverse->TabIndex = 5;
      this->checkBoxReverse->Text = L"Auto Reverse";
      this->checkBoxReverse->UseVisualStyleBackColor = true;
      // 
      // EvalGraph
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(803, 488);
      this->Controls->Add(tableLayoutPanel1);
      this->Margin = System::Windows::Forms::Padding(4);
      this->Name = L"EvalGraph";
      this->Text = L"PolyFit";
      this->Load += gcnew System::EventHandler(this, &EvalGraph::EvalGraph_Load);
      tableLayoutPanel1->ResumeLayout(false);
      tableLayoutPanel1->PerformLayout();
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->polyfitChart))->EndInit();
      this->contextMenuStripFitLines->ResumeLayout(false);
      this->ResumeLayout(false);

    }
#pragma endregion
  private:


    System::Void buttonGraph_Click(System::Object^  sender, System::EventArgs^  e)
    {
      OpenFileDialog^ dialog = gcnew OpenFileDialog;

      if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
      {
        delete m_pTheMyEval;
        m_pTheMyEval = nullptr;

        delete p_theMyTournament;
        p_theMyTournament = nullptr;

        comboBoxNames->SelectedIndex = -1;
        comboBoxNames->Items->Clear();

        if (System::IO::Path::GetExtension(dialog->FileName)->ToLower() == ".csv")
        {
          std::string fileName = msclr::interop::marshal_as<std::string>(dialog->FileName);

          EvalStacker evals = EvalStacker::Parse(fileName);

          m_pTheMyEval = new EvalStacker(evals);

          GraphData();
        }
        else if (System::IO::Path::GetExtension(dialog->FileName)->ToLower() == ".pgn")
        {
          std::string fileName = msclr::interop::marshal_as<std::string>(dialog->FileName);

          auto tournament = PGNParser::parse(fileName);

          // Set UI Items
          p_theMyTournament = tournament.release();

          ListNames();

          comboBoxNames->SelectedIndex = 0;
        }
        else
        {
          throw gcnew Exception("Invalid file format. Must .csv or .pgn");
        }

      }
    }

    array<double>^ to_array(const std::vector<double>& vec)
    {
      using namespace System;
      using namespace System::Runtime::InteropServices;

      array<double>^ ret = gcnew array<double>(vec.size());
      Marshal::Copy((IntPtr)((void*)vec.data()), ret, 0, vec.size());
      return ret;
    }

    void ListNames()
    {
      if (p_theMyTournament == nullptr)
      {
        return;
      }

      List<Object ^> ^names = gcnew List<Object ^>();

      auto games = p_theMyTournament->GetGames(Stockfish);

      for (size_t i = 0; i < games.size(); i++ )
      {
        auto const &g = games[i];

        StringBuilder ^sb = gcnew StringBuilder(256);

        sb->Append(i + 1);
        sb->Append(". ");
        sb->Append(gcnew String(g->GetHeader().WhitePlayer.c_str()));
        sb->Append(" - ");
        sb->Append(gcnew String(g->GetHeader().BlackPlayer.c_str()));
        sb->Append("(");
        sb->Append(g->GetHeader().PlyCount);
        sb->Append(") => ");

        sb->Append(gcnew String(g->GetHeader().GetResult().c_str()));

        names->Add( sb->ToString() );
      }

      comboBoxNames->Items->AddRange(names->ToArray());

    }

    void GraphData()
    {
      if (m_pTheMyEval == nullptr)
      {
        return;
      }

      seriesPoly->Points->Clear();
      seriesExp->Points->Clear();
      seriesLog->Points->Clear();
      seriesPow->Points->Clear();


      EvalStacker const & evals = *m_pTheMyEval;

      std::vector<double> oX = comboBoxSide->SelectedIndex == 0 ? 
        evals.GetWhiteMoves() :
        evals.GetBlackMoves();

      std::vector<double> oY = comboBoxSide->SelectedIndex == 0 ?
        evals.GetWhiteEvalCurve() :
        evals.GetBlackEvalCurve();

      seriesBase->Points->DataBindXY(to_array(oX), to_array(oY));

      TrendDetector td(oX, oY);

      if( checkBoxReverse->Checked )
      { 
        td.FavorWhite();
      }

      if (toolStripMenuItemPolynomialFit->Checked)
      {
        // polynomial fitting
        std::vector<double> oFittedY = td.GetPolynomial3FitCurve();

        seriesPoly->Points->DataBindXY(to_array(oX), to_array(oFittedY));
      }

      if (toolStripMenuItemExpFit->Checked)
      {
        // Exponential Fit
        std::vector<double> ExpFit = td.GetExponentialFitCurve();

        seriesExp->Points->DataBindXY(to_array(oX), to_array(ExpFit));
      }

      if (toolStripMenuItemLogFit->Checked)
      {
        // Logarithmic Fit
        std::vector<double> LogFit = td.GetLogarithmFitCurve();

        seriesLog->Points->DataBindXY(to_array(oX), to_array(LogFit));
      }

      if (toolStripMenuItemPowFit->Checked)
      {
        // Pow Fit
        std::vector<double> PowFit = td.GetPowerLawFitCurve();

        seriesPow->Points->DataBindXY(to_array(oX), to_array(PowFit));
      }
    }

  private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

    GraphData();

  }

  private: System::Void EvalGraph_Load(System::Object^  sender, System::EventArgs^  e) {

    seriesBase = this->polyfitChart->Series[0];
    seriesPoly = this->polyfitChart->Series[1];
    seriesExp = this->polyfitChart->Series[2];
    seriesLog = this->polyfitChart->Series[3];
    seriesPow = this->polyfitChart->Series[4];

    comboBoxSide->SelectedIndex = 0;

    this->comboBoxSide->SelectedIndexChanged += gcnew System::EventHandler(this, &EvalGraph::comboBox1_SelectedIndexChanged);
    this->comboBoxNames->SelectedIndexChanged += gcnew System::EventHandler(this, &EvalGraph::comboBoxNames_SelectedIndexChanged);
    this->checkBoxReverse->CheckedChanged += gcnew System::EventHandler(this, &EvalGraph::checkBoxReverse_CheckedChanged);

    this->toolStripMenuItemPolynomialFit->CheckedChanged += gcnew System::EventHandler(this, &EvalGraph::toolStripMenuItemFit_CheckedChanged);
    this->toolStripMenuItemLogFit->CheckedChanged += gcnew System::EventHandler(this, &EvalGraph::toolStripMenuItemFit_CheckedChanged);
    this->toolStripMenuItemExpFit->CheckedChanged += gcnew System::EventHandler(this, &EvalGraph::toolStripMenuItemFit_CheckedChanged);
    this->toolStripMenuItemPowFit->CheckedChanged += gcnew System::EventHandler(this, &EvalGraph::toolStripMenuItemFit_CheckedChanged);
  }

  private: System::Void comboBoxNames_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

    // Set UI Items
    if (p_theMyTournament == nullptr ||
      comboBoxNames->SelectedIndex == -1)
    {
      return;
    }

    auto game = p_theMyTournament->GetGames(Stockfish)[comboBoxNames->SelectedIndex];


    delete m_pTheMyEval;
    m_pTheMyEval = new EvalStacker(game->EvalGraph());

    GraphData();
  }


  private: System::Void toolStripMenuItemFit_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

    GraphData();

  }

  private: System::Void checkBoxReverse_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {

    GraphData();

  }
};

}

