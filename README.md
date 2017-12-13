# Corrector Tester

Corrector tester is a quality testing tool. It's purpose is to test how well sequence correctors perform. Testing is done using synthetic or semi-synthetic data.

It consists of 4 programs:

- **Genome generator** - generates genome using pseudo-random generation
- **Sequence generator** - generates sequences out of genome data
- **Matrix generator** - generates matrix for sequence generation
- **Quality measurer** - computes the result statistics

## System and hardware requirements

- GCC 4.8.1 or newer
- Boost libraries installed

*Sequence generator* may use quite a lot of memory since all generated sequences are stored in memory before they are processed.

## Compilation and installation

**Installing Corrector Tester:**

1. On a command line, `cd` to the repository.
2. Run `make` to compile the programs.
3. Run `make install` to install Corrector Tester to standard path 
   (`/usr/local/bin`). You may need admin privileges for this step. 
   In that case, run `sudo make install` and enter admin password.
     - You can alternatively run `make here` to copy the programs to the main
       repository folder. (In folder `Build`.)
     - You can choose where to install the applications by providing variable
      `prefix`. I.e. `make install prefix=/usr` will install Corrector Tester in `/usr/bin` folder.
4. After `make install` programs should be available using commands:
   `genome_generator`, `sequence_generator`, `matrix_generator`, and 
   `quality_measurer`.

## How to use

It depends, whether you want to test using synthetic or semi-synthetic data.

**Synthetic testing:**

1. Use *Genome generator* to generate synthetic genome.
2. Use *Sequence generator* to generate sequences with errors.
    - *Matrix generator* may or may not be used in this step (explained later)
3. Use sequence corrector that you want to test on.
   Generate corrected sequences.
4. Use *Quality measurer* to compute statistics.

**Semi-synthetic testing:**

1. Prepare real genome that you want to use for testing (in [FASTA] format).
2. Use *Sequence generator* to generate sequences with errors.
3. Use sequence corrector that you want to test on.
   Generate corrected sequences.
4. Use *Quality measurer* to compute statistics.

All programs have `--help` option that will give you description and all configuration options.

```
genome_generator --help
```

[FASTA]: https://en.wikipedia.org/wiki/FASTA_format "FASTA format"

### Genome generator
Genome generator is a tool for generating pseudo-random genome fragments.
Output is saved in [FASTA] format.

Simplest way to generate genome is to provide `-l` option specifying the length of fragment. Providing only this option will generate 1 fragment of specified length. Result will be saved in `genome.fa` file.

```
genome_generator -l 4k
```

For ease of use, you can use suffixes for large numbers: 
k - kilo, m - mega, g - giga, t - tera.
`52m` means that the fragment should be 52 millions bases long.

Option `-n` specifies number of fragments to be generated. All fragments will have same length specified with `-l`.
Option `-f` specifies resulting file name (if you want different name).

```
genome_generator -l 32M -n 123 -f data.fa
```

### Sequence generator
Sequence generator does 2 things. It pseudo-randomly generates sequences out of genome data. Then it introduces errors/mismatches to those data to create "corrupted" data. Those corrupted data should then be used for correction.

Simplest configuration requires these options: 

- `-c` - sets the coverage. Coverage 3.5 means, that there will be enough 
  sequences to cover the genome three and a half times.
- `-l` - the length of sequences to be generated
- `-u` - uniform probability. This is probability of errors being introduced.
  This probability will be applied uniformly. Probability 1.3 means, that there will be 1.3 % of errors.

```
sequence_generator -c 2.5 -l 100 -u 1.3
```

Using this configuration, Sequence generator assumes, that genome is in working directory with name `genome.fa`. If it's in a different directory or has different name, use option `-g` to specify the file.

If you want more detailed error probability specification, use `-f` instead of `-u`. `-f` expects name of file with probability matrix that can be generated using *Matrix generator*.

```
sequence_generator -c 2.5 -l 100 -g data.fa -f matrix.txt
```

**Output:** Sequence generator outputs two files. Generated "corrupted" sequences in [FASTQ] format and map file. This map file is important for computing statistics. It maps sequences back to genome. Sequences are automatically saved to `corrupted.fastq` file and mapping information are saved to `seq-map.map`. To set your own names, use `-q` for sequences and `-m` for map info.

```
sequence_generator -c 2.5 -l 100 -u 1.3 -q genseq.fastq -m mapinfo.map
```

[FASTQ]: https://en.wikipedia.org/wiki/FASTQ_format "Fastq format"

### Matrix generator
Matrix generator generates 3 dimensional probability matrix for use in *Sequence generator*. 3 dimensional means that there is 2 dimensional matrix for every base in sequence. That means that probabilities can be set independently for every position in sequence. You can create your own matrix for *Sequence generator* instead of using *Matrix generator*.

Program requires 2 options:

- `-l` - matrix length. This is the third dimension. Matrix length should correspond to the length of the longest sequence in your dataset.
- `-m` - error mean in %

```
matrix_generator -l 10 -m 3
```

Every base position will have different 2 dimensional matrix and thus different probability of errors.

Output is saved to `matrix.txt`. Use option `-f` to specify a different name.

### Quality measurer
Quality measurer computes statistics describing quality of corrections. It uses genome file, corrupted sequences, corrected sequences, and map file for computation.

Simplest configuration doesn't require any options:

```
quality_measurer
```

In this form, program assumes, that genome file name is `genome.fa`, corrupted sequences are stored in `corrupted.fastq`, corrected sequences are stored in `corrected.fastq`, and mapping info is stored in `seq-map.map`.

Files can be changed using options:

- `-g` for genome
- `-c` for corrupted sequences
- `-r` for corrected sequences
- `-m` for mapping file

```
quality_measurer -g data.fa -c genseq.fastq -r result.fastq -m mapinfo.map
```

Result is printed to the screen. If you want to save it to the file, use `-f` option.

```
quality_measurer -f result.txt
```

## Author
This implementation was created by **Miloš Šimek** at
[Masaryk University](https://www.muni.cz/en).  
You can email me at <simek@ics.muni.cz>.

## Acknowledgments
This work was supported by projects CERIT Scientific Cloud (LM2015085), CESNET (LM2015042), and ELIXIR-CZ project (LM2015047) in the programme "Projects of Large Research, Development, and Innovations Infrastructures".

I would like to thank Aleš Křenek who made this project possible.

Cite **TODO** when using this work.

## Copyright
This project is licensed under Creative Commons 4.0 International
license (CC BY 4.0). Summary of the license can be found
[here][lshort], the license itself can be found [here][llong].

[lshort]: https://creativecommons.org/licenses/by/4.0/
[llong]:  https://creativecommons.org/licenses/by/4.0/legalcode