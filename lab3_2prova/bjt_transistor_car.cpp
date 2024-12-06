#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <cmath>
#include <iostream>
#include <TStyle.h>
#include <fstream>

void BJT_Output_Characteristics_Table() {
    // Dati simulati: VCE, IC, e errori variabili
    const int nPoints = 26;
    double VCE[nPoints] = {0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.68, 0.76, 0.84,
                           0.92, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.5, 3.8, 4.0};
    double IC_140uA[nPoints] = {0.89, 4.15, 13.85, 17.10, 18.04, 18.38, 18.6, 18.74, 19.0, 19.15, 19.25,
                                19.35, 19.72, 19.92, 20.14, 20.32, 20.4, 20.7, 20.97, 21.18, 21.42, 21.6, 21.9, 22.14, 22.39, 22.2};
    double IC_70uA[nPoints] = {0.41, 2.51, 8.3, 9.26, 9.4, 9.46, 9.53, 9.57, 9.61, 9.65,
                                9.68, 9.73, 9.81, 9.9, 9.98, 10.07, 10.15,10.25, 10.36, 10.46, 10.54, 10.61, 10.7, 10.79, 10.87, 10.91};
    double errVCE[nPoints] = {0.01, 0.02, 0.03, 0.03, 0.04, 0.05, 0.05, 0.05, 0.06, 0.06,
                              0.07, 0.07, 0.08, 0.08, 0.09, 0.09, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1};
    double errIC_140[nPoints] = {0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 0.9, 1.0, 1.1, 1.2,
                             1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7};
    double errIC_70[nPoints] = {0.2, 0.3, 0.4, 0.5, 0.6, 0.8, 0.9, 1.0, 1.1, 1.2,
                             1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7}; 

                             
    // Creazione del canvas
    TCanvas *c1 = new TCanvas("c1", "Caratteristica di uscita del BJT", 800, 600);
    gPad->SetGrid();

    // Creazione dei grafici
    TGraphErrors *graph_140uA = new TGraphErrors(nPoints, VCE, IC_140uA, errVCE, errIC_140);
    graph_140uA->SetMarkerStyle(20);
    graph_140uA->SetMarkerColor(kBlue);
    graph_140uA->SetLineColor(kBlue);
    graph_140uA->SetTitle("Caratteristica di uscita del BJT; Tensione fra collettore ed emettitore (V_{CE}) [V]; Corrente di collettore (I_{C}) [mA]");

    TGraphErrors *graph_70uA = new TGraphErrors(nPoints, VCE, IC_70uA, errVCE, errIC_70);
    graph_70uA->SetMarkerStyle(21);
    graph_70uA->SetMarkerColor(kRed);
    graph_70uA->SetLineColor(kRed);

    // Disegna il grafico
    graph_140uA->Draw("AP");
    graph_70uA->Draw("P SAME");

    // Fit lineare nella regione attiva (VCE >= 1 V)
    TF1 *fitFunc140 = new TF1("fitFunc140", "[0] + [1]*x", 1.0, 4.0);
    TF1 *fitFunc70 = new TF1("fitFunc70", "[0] + [1]*x", 1.0, 4.0);
    graph_140uA->Fit(fitFunc140, "R");
    graph_70uA->Fit(fitFunc70, "R");
    fitFunc140->SetParName(0,"a_{140}");
    fitFunc140->SetParName(1,"b_{70}");
    fitFunc70->SetParName(0,"a_{140}");
    fitFunc70->SetParName(1,"b_{140}");

    // Ottenere i parametri del fit
    double a140 = fitFunc140->GetParameter(0);
    double b140 = fitFunc140->GetParameter(1);
    double a140_err = fitFunc140->GetParError(0);
    double b140_err = fitFunc140->GetParError(1);

    double a70 = fitFunc70->GetParameter(0);
    double b70 = fitFunc70->GetParameter(1);
    double a70_err = fitFunc70->GetParError(0);
    double b70_err = fitFunc70->GetParError(1);

    // Calcolo di g e beta
    double g140 = 1.0 / b140; // conduttanza (b200^{-1})
    double g70 = 1.0 / b70;
    double Ic_140uA_in3V = 21.6; // Define or calculate this value
    double Ic_70uA_in3V = 10.61; // Define or calculate this value
    double beta = fabs(Ic_140uA_in3V - Ic_70uA_in3V) / 70*1e-3; // valore assoluto di (Ic_140uA - Ic_70uA) diviso per la differenza di corrente di base (70uA)
    double beta_err = 1.0; // errore arbitrario
    
    // Creazione della tabella dei risultati
    TPaveText *table = new TPaveText(0.6, 0.6, 0.9, 0.9, "NDC");
    table->SetBorderSize(1);
    table->SetFillColor(0);
    table->SetTextAlign(12);
    table->AddText(Form("V_{a140} = %.2f #pm %.2f V", a140, a140_err));
    table->AddText(Form("r_{usc_140} = %.2f #pm %.2f V/mA", b140, b140_err));
    table->AddText(Form("V_{a70} = %.2f #pm %.2f V", a70, a70_err));
    table->AddText(Form("r_{usc_70} = %.2f #pm %.2f V/mA", b70, b70_err));
    table->AddText(Form("g_{140} = %.2f mA/V", g140));
    table->AddText(Form("g_{70} = %.2f mA/V", g70));
    table->AddText(Form("Beta = %.2f #pm %.2f", beta, beta_err));
    table->Draw();
    gStyle->SetOptFit(1111);

    // Legenda
    TLegend *legend = new TLegend(0.15, 0.7, 0.4, 0.85);
    legend->AddEntry(graph_140uA, "I_{B} = -140 uA", "p");
    legend->AddEntry(graph_70uA, "I_{B} = -70 uA", "p");
    legend->Draw();

    // Salva il grafico
    c1->SaveAs("BJT_Output_Characteristics_Table.png");
}
