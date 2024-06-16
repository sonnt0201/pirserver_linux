#include "export.h"

HANDLER turnOffRecording = [](Request* req, Response *res, bool *next) {
    pirOrm.setDataWrite(false);

    res->asPlainText("Pir recording is off");
    
};