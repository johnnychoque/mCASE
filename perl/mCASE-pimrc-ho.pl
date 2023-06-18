#!/usr/bin/perl -w

# Ejecuta mCASE modificando el peso del HO desde 0 a 1 con pasos de 0.05, mientras el precio va en orden inverso complementado el HO 
# para que ambos sumen 1. A los demas pesos se les asigna 0. En mCASE.cfg se fija el numero de usuarios a 150 y la relacion de precio entre
# consumer y business tambien se fija a 0.70 (valor intermedio entre 0.60 y 0.80 utilizado en otras simulaciones).

use Time::Local;

$cfgFile = "configs/mCASE-pimrc-ho.cfg";
$totSimulation_ = 100;

#@prefOP_  = (0.00, 0.00, 0.00, 0.00);
#@handover_= (0.00, 0.00, 0.20, 0.20);
#@quality_ = (0.00, 0.00, 0.00, 0.00);
#@load_    = (0.00, 1.00, 0.00, 0.80);
#@price_   = (1.00, 0.00, 0.80, 0.00);

sub changeAccessSelectionStrategy
{
    my($handover_, $price_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /CONSTRAINTS/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /OPERATOR=/ && $flag_ == 1) {
			$aux = sprintf("OPERATOR=%.2f\n",0);
			print OUT_FILE $aux;
		} elsif($_ =~ /HANDOVER=/ && $flag_ == 1) {
			$aux = sprintf("HANDOVER=%.2f\n",$handover_);
			print OUT_FILE $aux;
		} elsif($_ =~ /QUALITY=/ && $flag_ == 1) {
			$aux = sprintf("QUALITY=%.2f\n",0);
			print OUT_FILE $aux;
		} elsif($_ =~ /LOAD=/ && $flag_ == 1) {
			$aux = sprintf("LOAD=%.2f\n",0);
			print OUT_FILE $aux;
		} elsif($_ =~ /PRICE=/ && $flag_ == 1) {
			$aux = sprintf("PRICE=%.2f\n",$price_);
			print OUT_FILE $aux;
		} else {
			print OUT_FILE $_;
		}
	}

    close(CFG_FILE);
    close(OUT_FILE);
    $command = "rm -f ".$cfgFile;
    system($command);
    $command = "mv ".$tempFILE." ".$cfgFile;
    system($command);

}

sub changeSimulationNumbers
{
    my($numStrategy_, $numSimulation_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>) {
		if($_ =~ /\[*\]/) {
			if($_ =~ /SIMULATION/) {
				$flag_ = 1; 
			} else {
				$flag_ = 0;
			}
		}

		if($_ =~ /STRATEGY=/ && $flag_ == 1) {
			$aux = sprintf("STRATEGY=%03d\n",$numStrategy_);
			print OUT_FILE $aux;
		} elsif($_ =~ /NUMSIM=/ && $flag_ == 1) {
			$aux = sprintf("NUMSIM=%03d\n",$numSimulation_);
			print OUT_FILE $aux;
		} else {
			print OUT_FILE $_;
		}

    }
	
	close(CFG_FILE);
    close(OUT_FILE);
    $command = "rm -f ".$cfgFile;
    system($command);
    $command = "mv ".$tempFILE." ".$cfgFile;
    system($command);
}

sub executeSimulation
{
    my($numStr_,$numSim_) = @_;
    my($command,$path,$numStr,$numSim);
    
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);

	$path = "results/stats/";
	$numStr = sprintf("%03d",$numStr_);
	$numSim = sprintf("%03d",$numSim_);
	
#	$command = "mv ".$path."serviceStats.out ".$path.$numStr."_serviceStats_".$numSim.".out";
#	system($command);
#	$command = "mv ".$path."userStats.out ".$path.$numStr."_userStats_".$numSim.".out";
#	system($command);
#	$command = "mv ".$path."linkQualityStats.out ".$path.$numStr."_linkQualityStats_".$numSim.".out";
#	system($command);	
	$command = "mv ".$path."hoPerServStats.out ".$path.$numStr."_hoPerServStats_".$numSim.".out";
	system($command);
#	$command = "mv ".$path."hoPerUserStats.out ".$path.$numStr."_hoPerUserStats_".$numSim.".out";
#	system($command);	
#	$command = "mv ".$path."loadStats.out ".$path.$numStr."_loadStats_".$numSim.".out";
#	system($command);	
#	$command = "mv ".$path."paramsList.out ".$path.$numStr."_paramsList_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."priceList.out ".$path.$numStr."_priceList_".$numSim.".out";
	system($command);
#	$command = "mv ".$path."pricePerServStats.out ".$path.$numStr."_pricePerServStats_".$numSim.".out";
#	system($command);
#	$command = "mv ".$path."pricePerUserStats.out ".$path.$numStr."_pricePerUserStats_".$numSim.".out";
#	system($command);
	
}

# The main!!!
$t_ini = localtime;

for ($ho = 0; $ho <= 20; $ho++) {
	$price = 1.0-$ho*0.05;
	&changeAccessSelectionStrategy($ho*0.05,$price);
	for($numSim = 1 ; $numSim <= $totSimulation_ ; $numSim++) {
		printf("\n\nRunning mCASE for handover %.2f simulation %d\n",$ho*0.05,$numSim);
		&changeSimulationNumbers($ho,$numSim);
		&executeSimulation($ho,$numSim);
	}
}

$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
