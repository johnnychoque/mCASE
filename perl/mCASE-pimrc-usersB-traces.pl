#!/usr/bin/perl -w

use Time::Local;

$cfgFile = "configs/mCASE-pimrc-usersB-snapshot.cfg";
$totSimulation_ = 5;

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
    my($nPerc_,$numSim_) = @_;
	my($command,$path,$numSS,$auxSS,$auxUser,$auxSim);
	
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);

	$path = "results/snapshots/";	
	for($numSS=1; $numSS <= 360; $numSS++) {
		$auxSS = sprintf("%04d",$numSS);
		$auxPerc = sprintf("%03d",$nPerc_);
		$auxSim = sprintf("%03d",$numSim_);
		$command = "mv ".$path."snapshot_".$auxSS.".txt ".$path."caseB/B_UT_".$auxPerc."_".$auxSim."_".$auxSS.".tr";
		#printf("%s\n",$command);
		system($command);
	}
}


# The main!!!
$t_ini = localtime;

for($nPerc = 1 ; $nPerc <= 9 ; $nPerc++) {
	&changePercentConsumer(1.0-$nPerc*0.1);
	&changePercentBusiness($nPerc*0.1);
	for($numSim = 1 ; $numSim <= $totSimulation_ ; $numSim++) {
		printf("\n\nRunning mCASE for bussines %.2f simulation %d\n",$nPerc*0.1,$numSim);	
		&executeSimulation($nPerc*10,$numSim);
	}
}


$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
