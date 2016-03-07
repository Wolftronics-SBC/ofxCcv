#pragma once

#include "ofMain.h"

extern "C" {
#include "ccv.h"
}

ccv_dense_matrix_t toCcv(const ofPixels& pix);
ccv_dense_matrix_t toCcv(const ofBaseHasPixels& pix);


class ofxCcv {
private:
    ccv_convnet_t* convnet;
    vector<string> words;
    int nLayers;
    vector<string> layerNames;
    bool loaded;
    
public:
    class Classification {
    public:
        int imageNetId;
        string imageNetName;
        int rank;
        float confidence;
    };
    
    class FeatureMap {
    public:
        vector<float> acts;
        int rows;
        int cols;
        float max;
        void getImage(ofImage & img, bool autoBrighten=true);
    };
        
    ofxCcv();
    ~ofxCcv();
    void setup(string network);
    bool isLoaded() {return loaded;}
    
    int numLayers() {return nLayers;}
    vector<string> & getLayerNames() {return layerNames;}
    vector<ofxCcv::FeatureMap> getFeatureMaps(int layer);
    vector<ofImage> getWeights();
    vector<string> & getClasses() {return words;}
    
    vector<float> encode(ofPixels & img, int layer);
    vector<float> encode(ofBaseHasPixels & img, int layer) {return encode(img.getPixels(), layer);}
    
    template <class T>
    vector<Classification> classify(const T& img, int maxResults = 5) {
        vector<Classification> results;
        ccv_dense_matrix_t image;
        image = toCcv(img);
        ccv_dense_matrix_t* input = 0;
        ccv_convnet_input_formation(convnet->input, &image, &input);
        ccv_array_t* rank = 0;
        ccv_convnet_classify(convnet, &input, 1, &rank, maxResults, 1);
        int i;
        for (i = 0; i < rank->rnum; i++) {
            ccv_classification_t* classification = (ccv_classification_t*)ccv_array_get(rank, i);
            Classification result;
            result.imageNetId = classification->id + 1;
            result.imageNetName = words[classification->id];
            result.confidence = classification->confidence;
            result.rank = i;
            results.push_back(result);
        }
        ccv_array_free(rank);
        ccv_matrix_free(input);
        return results;
    }
    
    
    
    /////////
    ofImage blah(ofPixels & img, int layer);
    
};
