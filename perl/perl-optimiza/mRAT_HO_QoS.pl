#!/usr/bin/perl -w

use Time::Local;

$cfgFile = "configs/mCASE-scenario.cfg";
$numTraces_ = 100;
$numSnapshot_ = 200;
$gammaStrategy_ = 1;

$alpha_ = 0.1;
$beta_  = 0.0;
$gamma_ = 0.1;
$delta_ = 0.8;

sub changeAccessSelectionStrategy
{
    my($alpha_,$beta_,$gamma_,$delta_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
	if($_ =~ /\[*\]/) {
	    if($_ =~ /VALUES/) {
		$flag_ = 1; 
	    } else {
		$flag_ = 0;
	    }
	}
	if($_ =~ /ALPHA=/ && $flag_ == 1) {
	    $aux = sprintf("ALPHA=%.2f\n",$alpha_);
	    print OUT_FILE $aux;
	} elsif($_ =~ /BETA=/ && $flag_ == 1) {
	    $aux = sprintf("BETA=%.2f\n",$beta_);
	    print OUT_FILE $aux;
	} elsif($_ =~ /GAMMA=/ && $flag_ == 1) {
	    $aux = sprintf("GAMMA=%.2f\n",$gamma_);
	    print OUT_FILE $aux;
	} elsif($_ =~ /DELTA=/ && $flag_ == 1) {
	    $aux = sprintf("DELTA=%.2f\n",$delta_);
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

sub changeTraceNumber
{
    my($currenti_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
	if($_ =~ /\[*\]/) {
	    if($_ =~ /SNAPSHOTS/) {
		$flag_ = 1; 
	    } elsif($_ =~ /OPTIMIZER/) {
		$flag_ = 1;
	    } else {
		$flag_ = 0;
	    }
	}
	if($_ =~ /FILE=/ && $flag_ == 1) {
	    $aux = sprintf("FILE=%03d\n",$currenti_);
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
    my($currenti_) = @_;
    my($command);
    
    $command = "./bin/multiRATlp ".$cfgFile;
    printf("%s\n",$command);
    system($command);
}

sub moveResults
{
    my($currenti_,$currentj_) = @_;
    my($command_, $fileRoot_, $fileSrc_,$k_);
    
    $fileSrc_ = sprintf("%03d_datos_matlab.txt",$currenti_);	
    $fileRoot_ = sprintf("strategy_g%02d/%02d_%03d_datos_matlab.txt",$currentj_,$currentj_,$currenti_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    $fileSrc_ = sprintf("%03d_bs_matlab.txt",$currenti_);
    $fileRoot_ = sprintf("strategy_g%02d/%02d_%03d_bs_matlab.txt",$currentj_,$currentj_,$currenti_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    $fileSrc_ = sprintf("%03d_users_matlab.txt",$currenti_);
    $fileRoot_ = sprintf("strategy_g%02d/%02d_%03d_users_matlab.txt",$currentj_,$currentj_,$currenti_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    for($k_ = 1 ; $k_ <= $numSnapshot_; $k_++) {
    	$fileSrc_ = sprintf("%03d_final_solution_%04d.txt",$currenti_,$k_);
    	$fileRoot_ = sprintf("strategy_g%02d/%02d_%03d_final_solution_%04d.txt",$currentj_,$currentj_,$currenti_,$k_);
    	$command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    	system($command_);

	$fileSrc_ = sprintf("%03d_qos_final_%04d.txt",$currenti_,$k_);
        $fileRoot_ = sprintf("strategy_g%02d/%02d_%03d_qos_final_%04d.txt",$currentj_,$currentj_,$currenti_,$k_);
        $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
        system($command_);
    }
}

sub flushLogFiles
{
    my($currenti_) = @_;
    my($command_, $fileRoot_);
    
    $fileRoot_ = sprintf("%03d_printOutput_*.cfg",$currenti_);
    $command_ = "rm -f ".$fileRoot_;
    system($command_);

    $fileRoot_ = sprintf("%03d_solglpk_*.txt",$currenti_);
    $command_ = "rm -f ".$fileRoot_;
    system($command_);

    $fileRoot_ = sprintf("genera_mat_rest_*.cfg");
    $command_ = "rm -f ".$fileRoot_;
    system($command_);

    $fileRoot_ = sprintf("%03d_qos_red_*.txt",$currenti_);
    $command_ = "rm -f ".$fileRoot_;
    system($command_);
}

# The main!!!
	$t_ini = localtime;
	&changeAccessSelectionStrategy($alpha_, $beta_, $gamma_, $delta_);

	# I go through all cases
	for($i = 1 ; $i <= $numTraces_ ; $i++) {
   
		&changeTraceNumber($i);

		&executeSimulation($i);

		&moveResults($i,$gammaStrategy_);

		&flushLogFiles($i);

	}
	$t_fin = localtime;
	print "Tiempo inicio ",$t_ini,"\n";
	print "Tiempo final  ",$t_fin,"\n";
	#print "Diferencia ",$t_fin-$t_ini,"\n";

