
import pandas as pd
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import LabelEncoder
from keras.preprocessing.text import Tokenizer
from keras.preprocessing.sequence import pad_sequences
from keras.layers import Embedding
from keras.models import Sequential
from keras.layers import LSTM
from keras.layers import Dense
from keras.models import model_from_json
from keras import backend as K
import numpy as np

##############################
import sys

#read the news
f = open(sys.argv[1])
news = f.read()
print(news)


# load json and create model
json_file = open('model.json', 'r')
loaded_model_json = json_file.read()
json_file.close()
model = model_from_json(loaded_model_json)
#print(model.summary())
# load weights into new model
model.load_weights("model.h5")
#print("Loaded model from disk")

tokenizer = Tokenizer() 
tokenizer.fit_on_texts(news)
sequences = tokenizer.texts_to_sequences(news)
padded = pad_sequences(sequences, maxlen = 1000) 
prediction = model.predict(padded)
y_classes = prediction.argmax(axis=-1)
if(max(y_classes)):
    print("This is real news.")
else:
    print("This is fake news.")

K.clear_session()



#prediction = model.predict(np.array(news))

