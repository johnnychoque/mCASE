#!/usr/bin/perl -w

$cfgFile = "configs/mCASE-scenario.cfg";
$numTraces_ = 20;

# Definiendo bloques de 6 estrategias
@strategiesIni_ = (1, 7,  13, 19, 25, 30, 36, 42, 47, 52);
@strategiesFin_ = (6, 12, 18, 24, 29, 35, 41, 46, 51, 55);

# !! SOLO ELEGIR ESTE VALOR!! desde 1 hasta 10 y crear el directorio results/strategy_3D
$numPC_ = 1;

@alpha_ = (0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);
@beta_  = (0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.4, 0.4, 0.4, 0.4, 0.4, 0.4, 0.5, 0.5, 0.5, 0.5, 0.5, 0.6, 0.6, 0.6, 0.6, 0.7, 0.7, 0.7, 0.8, 0.8, 0.9);
@gamma_ = (0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.0, 0.1, 0.2, 0.3, 0.4, 0.0, 0.1, 0.2, 0.3, 0.0, 0.1, 0.2, 0.0, 0.1, 0.0);
@delta_ = (0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0, 0.5, 0.4, 0.3, 0.2, 0.1, 0.0, 0.4, 0.3, 0.2, 0.1, 0.0, 0.3, 0.2, 0.1, 0.0, 0.2, 0.1, 0.0, 0.1, 0.0, 0.0);

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

    $betaIdx_  = $beta_[$currentj_-1]*10;
    $gammaIdx_ = $gamma_[$currentj_-1]*10;
    $deltaIdx_ = $delta_[$currentj_-1]*10;
    
    $fileSrc_ = sprintf("%03d_datos_matlab.txt",$currenti_);
    $fileRoot_ = sprintf("strategy_3D/b%1d_g%1d_d%1d_%03d_datos_matlab.txt",$betaIdx_,$gammaIdx_,$deltaIdx_,$currenti_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    $fileSrc_ = sprintf("%03d_bs_matlab.txt",$currenti_);
    $fileRoot_ = sprintf("strategy_3D/b%1d_g%1d_d%1d_%03d_bs_matlab.txt",$betaIdx_,$gammaIdx_,$deltaIdx_,$currenti_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    $fileSrc_ = sprintf("%03d_users_matlab.txt",$currenti_);
    $fileRoot_ = sprintf("strategy_3D/b%1d_g%1d_d%1d_%03d_users_matlab.txt",$betaIdx_,$gammaIdx_,$deltaIdx_,$currenti_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    for($k_ = 1 ; $k_ <= 200; $k_++) {
    	$fileSrc_ = sprintf("%03d_final_solution_%04d.txt",$currenti_,$k_);
    	$fileRoot_ = sprintf("strategy_3D/b%1d_g%1d_d%1d_%03d_final_solution_%04d.txt",$betaIdx_,$gammaIdx_,$deltaIdx_,$currenti_,$k_);
    	$command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    	system($command_);

	$fileSrc_ = sprintf("%03d_qos_final_%04d.txt",$currenti_,$k_);
        $fileRoot_ = sprintf("strategy_3D/b%1d_g%1d_d%1d_%03d_qos_final_%04d.txt",$betaIdx_,$gammaIdx_,$deltaIdx_,$currenti_,$k_);
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

$straIni_ = $strategiesIni_[$numPC_-1];
$straFin_ = $strategiesFin_[$numPC_-1];

for($j = $straIni_ ; $j <= $straFin_ ; $j++) {
	&changeAccessSelectionStrategy($alpha_[$j-1], $beta_[$j-1], $gamma_[$j-1], $delta_[$j-1]);

	# I go through all cases
	for($i = 1 ; $i <= $numTraces_ ; $i++) {
		&changeTraceNumber($i);
		&executeSimulation($i);
		&moveResults($i,$j);
		&flushLogFiles($i);
	}
}

