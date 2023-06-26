import os
import tempfile

import matplotlib.pyplot as plt
import numpy as np

import pandas
import pyabc

pyabc.settings.set_figure_params('pyabc')

toymodel = __import__('toy_model')


def model(parameter):
    out = {"data": toymodel.model(parameter)}
    return out


def distance(x, x0):
    dist = (x["data"] - x0["data"]) ** 2
    return dist.sum()


prior = pyabc.Distribution(
    alpha=pyabc.RV("uniform", 0, 4),
    beta=pyabc.RV("uniform", 0, 1)
)



# def model(parameter):
#     return {"data": parameter["mu"] + 0.5 * np.random.randn()}
#
#
# prior = pyabc.Distribution(mu=pyabc.RV("uniform", 0, 5))
#
#
# def distance(x, x0):
#     return abs(x["data"] - x0["data"])


abc = pyabc.ABCSMC(model, prior, distance, population_size=1000)

db_path = os.path.join(tempfile.gettempdir(), "test.db")
observation = pandas.read_csv("data_observed.csv")['A']
abc.new("sqlite:///" + db_path, {"data": observation})

history = abc.run(minimum_epsilon=0.1, max_nr_populations=10)

fig, ax = plt.subplots()
for t in range(history.max_t + 1):
    df, w = history.get_distribution(m=0, t=t)
    pyabc.visualization.plot_kde_1d(
        df,
        w,
        xmin=0,
        xmax=5,
        x="mu",
        xname=r"$\mu$",
        ax=ax,
        label=f"PDF t={t}",
    )
ax.axvline(observation, color="k", linestyle="dashed")
ax.legend()

fig, arr_ax = plt.subplots(1, 3, figsize=(12, 4))

pyabc.visualization.plot_sample_numbers(history, ax=arr_ax[0])
pyabc.visualization.plot_epsilons(history, ax=arr_ax[1])
pyabc.visualization.plot_effective_sample_sizes(history, ax=arr_ax[2])

fig.tight_layout()
