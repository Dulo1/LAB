#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TPaveText.h>
#include <cmath>
#include <iostream>

void BJT_Output_Characteristics_Table() {
    // Dati simulati: VCE, IC, e errori variabili
    const int nPoints = 25;
    double VCE[nPoints] = {0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, 1.5, 1.7, 1.9,
                           2.1, 2.3, 2.5, 2.7, 2.9, 3.0, 3.3, 3.5, 3.7, 3.9, 4.1, 4.3, 4.5, 4.7, 4.9};
    double IC_140uA[nPoints] = {0.5, 2, 4, 6, 9, 14, 19, 22, 24, 25,
                                26, 27, 27.5, 28, 28.3, 28.6, 29, 29.2, 29.4, 29.5, 29.6, 29.7, 29.8, 29.9, 30};
    double IC_70uA[nPoints] = {0.25, 1, 2, 3, 4.5, 7, 10, 12, 13, 14,
                                14.5, 14.8, 15, 15.2, 15.3, 15.4, 15.5, 15.6, 15.7, 15.8, 15.9, 16, 16.1, 16.2, 16.3};
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
    fitFunc140->SetParName(1,"b_{70}");

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
    double Ic_140uA_in3V = 28.6; // Define or calculate this value
    double Ic_70uA_in3V = 15.4; // Define or calculate this value
    double beta = fabs(Ic_140uA_in3V - Ic_70uA_in3V); // valore assolto di Ic_200uA-Ic_100uA in VCE = 3 V
    double beta_err = 1.0; // errore arbitrario
    
    // Creazione della tabella dei risultati
    TPaveText *table = new TPaveText(0.6, 0.6, 0.9, 0.9, "NDC");
    table->SetBorderSize(1);
    table->SetFillColor(0);
    table->SetTextAlign(12);
    table->AddText(Form("a_{200} = %.2f #pm %.2f V", a140, a140_err));
    table->AddText(Form("b_{200} = %.2f #pm %.2f V/mA", b140, b140_err));
    table->AddText(Form("a_{70} = %.2f #pm %.2f V", a70, a70_err));
    table->AddText(Form("b_{70} = %.2f #pm %.2f V/mA", b70, b70_err));
    table->AddText(Form("g_{140} = %.2f mA/V", g140));
    table->AddText(Form("g_{70} = %.2f mA/V", g70));
    table->AddText(Form("Beta = %.2f #pm %.2f", beta, beta_err));
    table->Draw();

    // Legenda
    TLegend *legend = new TLegend(0.15, 0.7, 0.4, 0.85);
    legend->AddEntry(graph_140uA, "I_{B} = -140 uA", "p");
    legend->AddEntry(graph_70uA, "I_{B} = -70 uA", "p");
    legend->Draw();

    // Salva il grafico
    c1->SaveAs("BJT_Output_Characteristics_Table.png");
}
