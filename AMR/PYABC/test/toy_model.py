import numpy as np

def model(parameters):
    """simple model for demo"""

    alpha = parameters['alpha']
    beta = parameters['beta']
    result = [np.random.normal(loc=0, scale=1, size=1)[0] + alpha + beta, np.random.normal(loc=0, scale=1, size=1)[0] + alpha + beta]
    return result
