import datalab

train_str = datalab.storage.Bucket('2065_aap75_scs89').item("07_16_50pts.csv").read_from()
test_str = datalab.storage.Bucket('2065_aap75_scs89').item("goodTestData_7subs.csv").read_from()
wordlist_str = datalab.storage.Bucket('2065_aap75_scs89').item('wordlist_2k.csv').read_from()

import tensorflow as tf
import numpy as np

sess = tf.InteractiveSession()

wordlist = []
for line in wordlist_str.split('\n'):
    if line:
        wordlist.append(line)

LABELS = "gaming SandersForPresident The_Donald trees gonewild BlackPeopleTwitter me_irl".split()

import collections

def read_datafile(filestr, numrows, numcols):
    features = np.zeros((numrows, numcols))
    labels = np.zeros((numrows, len(LABELS)))
    i = -1

    print "begin processing"
    for line in filestr.split('\n'):
        if not line: continue
        if i == -1:
          i = 0
          continue
        try:
            label, score, text  = line.split(",")
        except:
            print("BAD LINE"), line
            continue
        label = label.strip()
        words = [word.strip() for word in text.split()]
        word_freqs = collections.Counter(words)

        #utt_features = [word_freqs.get(word, 0) for word in wordlist]
        features[i] = [word_freqs.get(word, 0) for word in wordlist]
        #utt_labels = [int(l == label) for l in LABELS]
        labels[i] = [int(l == label) for l in LABELS]
        
        #features.append(utt_features)
        #labels.append(utt_labels)
        i = i + 1
        if i % 10000 == 0:
          print "processed", i
    print "finished processing"

    return features, labels

train_x, train_y = read_datafile(train_str, 45748, len(wordlist))
test_x, test_y = read_datafile(test_str, 384, len(wordlist))

x = tf.placeholder(tf.float32, [None, len(wordlist)])
y_ = tf.placeholder(tf.float32, [None, len(LABELS)])

w = tf.Variable(tf.zeros([len(wordlist), len(LABELS)]))
b = tf.Variable(tf.zeros([len(LABELS)]))
y = tf.nn.softmax(tf.matmul(x, w) + b)

cross_entropy = -tf.reduce_mean(y_ * tf.log(y))
correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

import random

random.seed(106)
batch_size = 64
max_num_bad_batches = 300

train_step = tf.train.FtrlOptimizer(0.8).minimize(cross_entropy)

#train
num_bad_batches = 0
best_test_ce = 10000
tf.initialize_all_variables().run()
for i in range(1000):
    #indices = range(train_x.shape[0])
    #random.shuffle(indices)
    indices = random.sample(xrange(train_x.shape[0]), batch_size)#indices[:batch_size]
    batch_xs, batch_ys = train_x[indices], train_y[indices]
    train_step.run({x: batch_xs, y_: batch_ys})
    train_ce = cross_entropy.eval({x: batch_xs, y_: batch_ys})
    test_ce = cross_entropy.eval({x: test_x, y_: test_y})

    if best_test_ce < test_ce:
        num_bad_batches += 1
    else:
        best_test_ce = test_ce
        num_bad_batches = 0
    if i % 100 == 0:
        print("CE train/test:", train_ce, test_ce, accuracy.eval({x: test_x, y_: test_y}))
    if num_bad_batches > max_num_bad_batches:
        break

#test
correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print("Accuracy:", accuracy.eval({x: test_x, y_: test_y}))
predictions = tf.argmax(y, 1)

def predict(check_str,score):
    check_x, check_y = read_datafile_custom("Q, "+ score +", " + check_str)
    return LABELS[predictions.eval({x: check_x, y_: check_y})[0]]

def print_predict(check_str, actual_str):
    print("Predicted: %s, Actual: %s" % (
        predict(check_str,"1"), actual_str
    ))


def read_datafile_custom(filestr):
    features = []
    labels = []

    for line in filestr.split('\n'):
        if not line: continue
        try:
            label, score, text  = line.split(",")
        except:
            print("BAD LINE"), line
            continue
        label = label.strip()
        words = [word.strip() for word in text.split()]
        word_freqs = collections.Counter(words)

        utt_features = [word_freqs.get(word, 0) for word in wordlist]
        utt_labels = [int(l == label) for l in LABELS]
        
        features.append(utt_features)
        labels.append(utt_labels)

    return np.array(features), np.array(labels)


