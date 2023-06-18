#!/usr/bin/perl -w

# Ejecuta mCASE modificando el parametro DISCOUNT del HO desde 0.01 a 0.99 con pasos de 0.05.
# Tambien se varia la relacion de precio entre consumer y business a 0.60 y 0.80
# mCASE.cfg solo debe asignar pesos de 0.5 a PRICE y HANDOVER, los demas pesos deben ser cero. Numero de usuarios 150

use Time::Local;
use Math::Round;

#$cfgFile = "configs/mCASE-wowmom-hoDisc-ratio.cfg";
$cfgFile = "configs/mCASE-wowmom-hoDisc-flat.cfg";
$totSimulation_ = 100;

sub changePriceConsumer
{
	my($prCon_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /USER0/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /PRICE=/ && $flag_ == 1) {
			$aux = sprintf("PRICE=%.2f\n",$prCon_);
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

sub changeHoDiscount
{
	my($hoDisc_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /HOPARAMS/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /DISCOUNT=/ && $flag_ == 1) {
			$aux = sprintf("DISCOUNT=%.2f\n",$hoDisc_);
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
    my($numStr_,$numParam_,$numSim_) = @_;
    my($command,$path,$numStr,$numParam,$numSim);
    
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);

	$path = "results/stats/";
	$numStr = sprintf("%03d",$numStr_);
	$numParam = sprintf("%03d",$numParam_);
	$numSim = sprintf("%03d",$numSim_);
	
#	$command = "mv ".$path."serviceStats.out ".$path.$numStr."_serviceStats_".$numSim.".out";
#	system($command);
#	$command = "mv ".$path."userStats.out ".$path.$numStr."_userStats_".$numSim.".out";
#	system($command);
#	$command = "mv ".$path."linkQualityStats.out ".$path.$numStr."_linkQualityStats_".$numSim.".out";
#	system($command);	
#	$command = "mv ".$path."hoPerServStats.out ".$path.$numStr."_hoPerServStats_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."hoPerUserStats.out ".$path.$numStr."_hoPerUserStats_".$numParam."_".$numSim.".out";
#	system($command);
	&runCommand($command);
#	$command = "mv ".$path."loadStats.out ".$path.$numStr."_loadStats_".$numSim.".out";
#	system($command);	
#	$command = "mv ".$path."paramsList.out ".$path.$numStr."_paramsList_".$numSim.".out";
#	system($command);
#	$command = "mv ".$path."priceList.out ".$path.$numStr."_priceList_".$numSim.".out";
#	system($command);
#	$command = "mv ".$path."pricePerServStats.out ".$path.$numStr."_pricePerServStats_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."pricePerUserStats.out ".$path.$numStr."_pricePerUserStats_".$numParam."_".$numSim.".out";
#	system($command);
	&runCommand($command);
	
}

sub runCommand
{
	my($command_) = @_;
	system($command_);
	
	if ( $? == -1 ) {
		print "\nCommand failed: $!\n";
		die "Please check the command ..."; 
	}
	elsif (($? >> 8) != 0) {
		print "\nSome parameter of the command is wrong\n";
		die "Please fix the wrong parameter";
	}

}

# The main!!!
$t_ini = localtime;
# ratio 
#@prCon_  = (0.60, 0.80);
#$totprCon_ = 2;
# flat
@prCon_  = (0.60);
$totprCon_ = 1;

#@hoDisc_ = (0.01,0.05,0.10,0.15,0.20,0.25,0.30,0.35,0.40,0.45,0.50,0.55,0.60,0.65,0.70,0.75,0.80,0.85,0.90,0.95,0.99);
#$totDisc_ = 21;
@hoDisc_ = (0.0001,0.00012441,0.00015477,0.00019255,0.00023954,0.00029801,0.00037074,0.00046123,0.0005738,0.00071385,0.00088808,0.0011048,0.0013745,0.00171,0.0021273,0.0026466,0.0032925,0.0040961,0.0050959,0.0063396,0.0078869,0.0098119,0.012207,0.015186,0.018892,0.023504,0.02924,0.036377,0.045255,0.056301,0.070042,0.087138,0.10841,0.13486,0.16778,0.20873,0.25968,0.32306,0.40191,0.5);
$totDisc_ = 40;

for ($j=0; $j<$totprCon_; $j++) {
	&changePriceConsumer($prCon_[$j]);
	for ($i=0; $i<$totDisc_; $i++) {
		&changeHoDiscount($hoDisc_[$i]);
		for($numSim=1; $numSim<=$totSimulation_; $numSim++) {
			printf("\n\nRunning mCASE for HO discount %d Price consumer %.2f Simulation %d\n",$i,$prCon_[$j],$numSim);
			&executeSimulation($prCon_[$j]*100,$i,$numSim);
		}
	}
}

$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
