#!/usr/bin/perl -w

$cfgFile = "configs/mCASE-prueba.cfg";
$totSimulation_ = 5;
$totStrategies_ = 3;

@alpha_ = (0.10, 0.10, 0.10);
@beta_  = (0.45, 0.45, 0.00);
@gamma_ = (0.45, 0.00, 0.45);
@delta_ = (0.00, 0.45, 0.45);

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

sub changeSimulationNumbers
{
    my($numStrategy_, $numSimulation_) = @_;
    my($tempFILE, $command, $aux, $outputFile, $flag_);

    $tempFILE='temp.cfg';

    open(CFG_FILE, "$cfgFile") or die "Cannot open CFG file";
    open(OUT_FILE,">$tempFILE") or die "Cannot open Temp file";

    $flag_ = 0;
    while(<CFG_FILE>){
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
		} else {
			print OUT_FILE $_;
		}

		if($_ =~ /FILE=/ && $flag_ == 1) {
			$aux = sprintf("FILE=%03d\n",$numSimulation_);
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
    
    $command = "./bin/mCASE ".$cfgFile;
    printf("%s\n",$command);
    system($command);
}

sub moveResults
{
    my($currenti_,$currentj_) = @_;
    my($command_, $fileRoot_, $fileSrc_,$k_);
    
    $fileSrc_ = sprintf("%03d_datos_matlab.txt",$currenti_);	
    $fileRoot_ = sprintf("strategy_%02d/%02d_%03d_datos_matlab.txt",$currentj_+5,$currentj_+5,$currenti_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    $fileSrc_ = sprintf("%03d_bs_matlab.txt",$currenti_);
    $fileRoot_ = sprintf("strategy_%02d/%02d_%03d_bs_matlab.txt",$currentj_+5,$currentj_+5,$currenti_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    $fileSrc_ = sprintf("%03d_users_matlab.txt",$currenti_);
    $fileRoot_ = sprintf("strategy_%02d/%02d_%03d_users_matlab.txt",$currentj_+5,$currentj_+5,$currenti_);
    $command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    system($command_);

    for($k_ = 1 ; $k_ <= 200; $k_++) {
    	$fileSrc_ = sprintf("%03d_final_solution_%04d.txt",$currenti_,$k_);
    	$fileRoot_ = sprintf("strategy_%02d/%02d_%03d_final_solution_%04d.txt",$currentj_+5,$currentj_+5,$currenti_,$k_);
    	$command_ = "mv ".$fileSrc_." results/".$fileRoot_;
    	system($command_);

	$fileSrc_ = sprintf("%03d_qos_final_%04d.txt",$currenti_,$k_);
        $fileRoot_ = sprintf("strategy_%02d/%02d_%03d_qos_final_%04d.txt",$currentj_+5,$currentj_+5,$currenti_,$k_);
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
for($j = 1 ; $j <= $totStrategies_ ; $j++) {
	&changeAccessSelectionStrategy($alpha_[$j-1], $beta_[$j-1], $gamma_[$j-1], $delta_[$j-1]);
	# I go through all cases
	for($i = 1 ; $i <= $totSimulation_ ; $i++) {
		&changeSimulationNumbers($j,$i);
#		&executeSimulation($i);
#		&moveResults($i,$j);
#		&flushLogFiles($i);
	}
}
