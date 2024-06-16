#include "export.h"

HANDLER turnOnRecording = [](Request *req, Response *res, bool *next) {
    pirOrm.setDataWrite(true);

    res->asPlainText(
        "Pir Recording is On"
    );
};