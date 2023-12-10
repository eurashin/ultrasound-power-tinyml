import matplotlib.pyplot as plt
import numpy as np

import argparse
from results_data import * 

def main():
    PLOT = {'stairs': (STAIRS_RESULTS, 'YlGn', 'hyperparameters-stairs.pdf'), 
            'all': (ALL_RESULTS, 'BuPu', 'hyperparameters.pdf'), 
            'sitting': (SITTING_RESULTS, 'OrRd', 'sitting.pdf')}

    parser = argparse.ArgumentParser(description='Read and store a string parameter from the terminal.')
    parser.add_argument('-d', '--data', type=str, help='String parameter to be stored', default="all")

    args = parser.parse_args()

    if args.data:
        data_variable = args.data
        print(f'The value of the data variable is: {data_variable}')
    else:
        print('No value provided for the data variable.')

    # Define the hyperparameter grid
    filters = ['None', 'Low', 'High']
    fft_lengths = [16, 64, 128, 256, 512]

    # Define plotting vals
    res, color, title = PLOT[data_variable]


    # Create a 2D array to store validation accuracies
    acc_matrix = np.zeros((len(filters), len(fft_lengths)))

    # Fill the matrix with validation accuracies
    for row in res:
        filter_idx = filters.index(row[0])
        fft_length_idx = fft_lengths.index(row[1])
        acc_matrix[filter_idx, fft_length_idx] = row[2]

    # Plot the grid with colored cells
    fig, ax = plt.subplots(figsize = (5, 5))
    cax = ax.matshow(acc_matrix, cmap = color)

    # Add colorbar
    #cbar = fig.colorbar(cax, aspect = 3)

    # Set axis labels and ticks with larger font size
    ax.set_xticks(np.arange(len(fft_lengths)))
    ax.set_yticks(np.arange(len(filters)))
    ax.set_xticklabels(fft_lengths, fontsize=20)
    ax.set_yticklabels(filters, fontsize=20)

    # Set axis labels with larger font size
    plt.xlabel('FFT Length', fontsize=20)
    plt.ylabel('Filter', fontsize=20)

    # Show the plot
    plt.savefig(title, bbox_inches = "tight")
    plt.show()



if __name__ == "__main__":
    main()

