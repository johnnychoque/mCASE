#!/usr/bin/perl -w

# El fichero mCASE.cfg debe tener configurado los siguientes valores para los pesos:
#
# [CONSTRAINTS]
# QUALITY=0.00
# HANDOVER=0.50
# OPERATOR=0.00
# LOAD=0.00
# PRICE=0.50

use Time::Local;

$cfgFile = "configs/mCASE-pimrc-usersB-C.cfg";
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

sub changePercentConsumer
{
	my($perCon_) = @_;
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
		if($_ =~ /PROBABILITY=/ && $flag_ == 1) {
			$aux = sprintf("PROBABILITY=%.2f\n",$perCon_);
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

sub changePercentBusiness
{
	my($perBus_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /USER1/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /PROBABILITY=/ && $flag_ == 1) {
			$aux = sprintf("PROBABILITY=%.2f\n",$perBus_);
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
    my($numStr_,$pBuss_,$numSim_) = @_;
    my($command,$path,$numStr,$pBuss,$numSim);
    
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);

	$path = "results/stats/";
	$numStr = sprintf("%03d",$numStr_);
	$pBuss = sprintf("%03d",$pBuss_);
	$numSim = sprintf("%03d",$numSim_);
	
#	$command = "mv ".$path."serviceStats.out ".$path.$numStr."_serviceStats_".$pBuss."_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."userStats.out ".$path.$numStr."_userStats_".$pBuss."_".$numSim.".out";
	system($command);
#	$command = "mv ".$path."linkQualityStats.out ".$path.$numStr."_linkQualityStats_".$pBuss."_".$numSim.".out";
#	system($command);	
#	$command = "mv ".$path."hoPerServStats.out ".$path.$numStr."_hoPerServStats_".$pBuss."_".$numSim.".out";
#	system($command);
#	$command = "mv ".$path."hoPerUserStats.out ".$path.$numStr."_hoPerUserStats_".$pBuss."_".$numSim.".out";
#	system($command);
#	$command = "mv ".$path."loadStats.out ".$path.$numStr."_loadStats_".$pBuss."_".$numSim.".out";
#	system($command);	
#	$command = "mv ".$path."paramsList.out ".$path.$numStr."_paramsList_".$pBuss."_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."priceList.out ".$path.$numStr."_priceList_".$pBuss."_".$numSim.".out";
	system($command);
#	$command = "mv ".$path."pricePerServStats.out ".$path.$numStr."_pricePerServStats_".$pBuss."_".$numSim.".out";
#	system($command);
	$command = "mv ".$path."pricePerUserStats.out ".$path.$numStr."_pricePerUserStats_".$pBuss."_".$numSim.".out";
	system($command);

}

# The main!!!
$t_ini = localtime;
for($prCon = 6 ; $prCon <= 8 ; $prCon++) {
	&changePriceConsumer($prCon*0.1);
	for($nPerc = 1 ; $nPerc <= 9 ; $nPerc++) {
		&changePercentConsumer(1.0-$nPerc*0.1);
		&changePercentBusiness($nPerc*0.1);
		for($nSim = 1 ; $nSim <= $totSimulation_ ; $nSim++) {
			printf("\n\nRunning mCASE for price consumer %.2f bussines %.2f simulation %d\n",$prCon*0.1,$nPerc*0.1,$nSim);
			&executeSimulation($prCon*10,$nPerc*10,$nSim);
		}
	}
}

$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
