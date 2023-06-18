#!/usr/bin/perl -w

use Time::Local;

$cfgFile = "configs/mCASE-pimrc-usersA.cfg";
$totSimulation_ = 100;
#$totStrategies_ = 2;

#@prefOP_  = (0.00, 0.00, 0.00, 0.00);
#@handover_= (0.00, 0.00, 0.20, 0.20);
#@quality_ = (0.00, 0.00, 0.00, 0.00);
#@load_    = (0.00, 1.00, 0.00, 0.80);
#@price_   = (1.00, 0.00, 0.80, 0.00);

#@handover_= (0.20, 0.50, 0.80);
#@price_   = (0.80, 0.50, 0.20);

sub changeAccessSelectionStrategy
{
#    my($handover_,$price_) = @_;
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
			$aux = sprintf("HANDOVER=%.2f\n",0.2);
			print OUT_FILE $aux;
		} elsif($_ =~ /QUALITY=/ && $flag_ == 1) {
			$aux = sprintf("QUALITY=%.2f\n",0);
			print OUT_FILE $aux;
		} elsif($_ =~ /LOAD=/ && $flag_ == 1) {
			$aux = sprintf("LOAD=%.2f\n",0);
			print OUT_FILE $aux;
		} elsif($_ =~ /PRICE=/ && $flag_ == 1) {
			$aux = sprintf("PRICE=%.2f\n",0.8);
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

sub changeNumUsers
{
	my($numUsers_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
		if($_ =~ /\[*\]/) {
			if($_ =~ /USERS/) {
			$flag_ = 1; 
			} else {
			$flag_ = 0;
			}
		}
		if($_ =~ /NUMBER=/ && $flag_ == 1) {
			$aux = sprintf("NUMBER=%d\n",$numUsers_);
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
    my($numStr_,$numUser_,$numSim_) = @_;
    my($command,$path,$numStr,$numUser,$numSim);
    
    $command = "./bin/mcase ".$cfgFile;
#    printf("%s\n",$command);
    system($command);

	$path = "results/stats/";
	$numStr = sprintf("%03d",$numStr_);
	$numUser = sprintf("%03d",$numUser_);
	$numSim = sprintf("%03d",$numSim_);
	
	$command = "mv ".$path."serviceStats.out ".$path.$numStr."_serviceStats_".$numUser."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."userStats.out ".$path.$numStr."_userStats_".$numUser."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."linkQualityStats.out ".$path.$numStr."_linkQualityStats_".$numUser."_".$numSim.".out";
	system($command);	
	$command = "mv ".$path."handoverStats.out ".$path.$numStr."_handoverStats_".$numUser."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."loadStats.out ".$path.$numStr."_loadStats_".$numUser."_".$numSim.".out";
	system($command);	
	$command = "mv ".$path."paramsList.out ".$path.$numStr."_paramsList_".$numUser."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."priceList.out ".$path.$numStr."_priceList_".$numUser."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."pricePerServStats.out ".$path.$numStr."_pricePerServStats_".$numUser."_".$numSim.".out";
	system($command);
	$command = "mv ".$path."pricePerUserStats.out ".$path.$numStr."_pricePerUserStats_".$numUser."_".$numSim.".out";
	system($command);

}

# The main!!!
$t_ini = localtime;
&changeAccessSelectionStrategy();
#for($numStr = 1 ; $numStr <= $totStrategies_ ; $numStr++) {
	#&changeAccessSelectionStrategy($handover_[$numStr-1], $price_[$numStr-1]);
for($prCon = 6 ; $prCon <= 8 ; $prCon++) {
	&changePriceConsumer($prCon*0.1);
	for($nUsers = 1 ; $nUsers <= 10 ; $nUsers++) {
		&changeNumUsers($nUsers*25);
		for($numSim = 1 ; $numSim <= $totSimulation_ ; $numSim++) {
			printf("\n\nRunning mCASE for price consumer %.2f users %d simulation %d\n",$prCon*0.1,$nUsers*25,$numSim);
			$numStr = $prCon;
			&changeSimulationNumbers($numStr,$numSim);
			&executeSimulation($prCon*10,$nUsers*25,$numSim);
		}
	}
}

$t_fin = localtime;
print "\n","Tiempo inicio de la simulacion ",$t_ini,"\n";
print "Tiempo final de la simulacion ",$t_fin,"\n";
