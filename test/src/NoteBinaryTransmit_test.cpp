/*!
 * @file NoteBinaryTransmit_test.cpp
 *
 * Written by the Blues Inc. team.
 *
 * Copyright (c) 2023 Blues Inc. MIT License. Use of this source code is
 * governed by licenses granted by the copyright holder including that found in
 * the
 * <a href="https://github.com/blues/note-c/blob/master/LICENSE">LICENSE</a>
 * file.
 *
 */

#ifdef NOTE_C_TEST

#include <catch2/catch_test_macros.hpp>
#include "fff.h"

#include "n_lib.h"

DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC(J *, NoteNewRequest, const char *)
FAKE_VALUE_FUNC(J *, NoteRequestResponse, J *)
FAKE_VALUE_FUNC(bool, NoteRequest, J *)
FAKE_VALUE_FUNC(const char *, NoteChunkedTransmit, uint8_t *, size_t, bool)
FAKE_VOID_FUNC(NoteLockNote)
FAKE_VOID_FUNC(NoteUnlockNote)

uint8_t buf[32] = {0xDE, 0xAD, 0xBE, 0xEF};
size_t dataLen = 4;
size_t bufLen = sizeof(buf);

namespace
{

SCENARIO("NoteBinaryTransmit")
{
    RESET_FAKE(NoteNewRequest);
    RESET_FAKE(NoteRequestResponse);
    RESET_FAKE(NoteRequest);
    RESET_FAKE(NoteChunkedTransmit);
    RESET_FAKE(NoteLockNote);
    RESET_FAKE(NoteUnlockNote);

    NoteSetFnDefault(malloc, free, NULL, NULL);

    NoteNewRequest_fake.custom_fake = [](const char *) -> J * {
        return JCreateObject();
    };

    GIVEN("The response to the initial card.binary request to get the length "
          "and max parameters indicates a problem") {

        AND_GIVEN("The response is NULL") {
            NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
                JDelete(req);

                return NULL;
            };

            WHEN("NoteBinaryTransmit is called") {
                const char *err = NoteBinaryTransmit(buf, dataLen, bufLen, 0);

                THEN("An error is returned") {
                    CHECK(err != NULL);
                }
            }
        }

        AND_GIVEN("The response has a generic error") {
            NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
                JDelete(req);
                J *rsp = JCreateObject();
                JAddStringToObject(rsp, "err", "some error");

                return rsp;
            };

            WHEN("NoteBinaryTransmit is called") {
                const char *err = NoteBinaryTransmit(buf, dataLen, bufLen, 0);

                THEN("An error is returned") {
                    CHECK(err != NULL);
                }
            }
        }

        AND_GIVEN("The max parameter is 0") {
            NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
                JDelete(req);
                J *rsp = JCreateObject();
                JAddIntToObject(rsp, "max", 0);

                return rsp;
            };

            WHEN("NoteBinaryTransmit is called") {
                const char *err = NoteBinaryTransmit(buf, dataLen, bufLen, 0);

                THEN("An error is returned") {
                    CHECK(err != NULL);
                }
            }
        }

        AND_GIVEN("The offset doesn't match the length returned by the "
                  "Notecard") {
            NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
                JDelete(req);
                J *rsp = JCreateObject();
                JAddIntToObject(rsp, "length", dataLen);
                JAddIntToObject(rsp, "max", dataLen * 2);

                return rsp;
            };

            WHEN("NoteBinaryTransmit is called") {
                const char *err = NoteBinaryTransmit(buf, dataLen, bufLen, 0);

                THEN("An error is returned") {
                    CHECK(err != NULL);
                }
            }
        }

        AND_GIVEN("The remaining space implied by max and length is too small "
                  "to hold the caller's data") {

            AND_GIVEN("The length parameter is 0") {
                NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
                    JDelete(req);
                    J *rsp = JCreateObject();
                    JAddIntToObject(rsp, "length", 0);
                    JAddIntToObject(rsp, "max", dataLen - 1);

                    return rsp;
                };

                WHEN("NoteBinaryTransmit is called") {
                    const char *err = NoteBinaryTransmit(buf, dataLen, bufLen,
                                                         0);

                    THEN("An error is returned") {
                        CHECK(err != NULL);
                    }
                }
            }

            AND_GIVEN("The length parameter is non-zero and the buffer is being"
                      " appended to existing data") {
                size_t currLen = dataLen;
                NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
                    JDelete(req);
                    J *rsp = JCreateObject();
                    JAddIntToObject(rsp, "length", dataLen);
                    JAddIntToObject(rsp, "max", dataLen * 2 - 1);

                    return rsp;
                };

                WHEN("NoteBinaryTransmit is called") {
                    const char *err = NoteBinaryTransmit(buf, dataLen, bufLen,
                                                         dataLen);

                    THEN("An error is returned") {
                        CHECK(err != NULL);
                    }
                }
            }
        }
    }

    GIVEN("The encoded length is exactly the same size as the "
          "buffer, leaving no room for the terminating newline") {
        NoteRequestResponse_fake.custom_fake = [](J *req) -> J * {
            JDelete(req);
            J *rsp = JCreateObject();
            JAddIntToObject(rsp, "length", 0);
            JAddIntToObject(rsp, "max", 1024);

            return rsp;
        };
        uint32_t newBufLen = cobsEncodedLength(buf, dataLen);

        WHEN("NoteBinaryTransmit is called") {
            const char *err = NoteBinaryTransmit(buf, dataLen, newBufLen, 0);

            THEN("An error is returned") {
                CHECK(err != NULL);
            }
        }
    }

    GIVEN("The initial card.binary response is ok") {
        auto cardBinaryRspInitial = [](J *req) -> J * {
            JDelete(req);
            J *rsp = JCreateObject();
            JAddIntToObject(rsp, "length", 0);
            JAddIntToObject(rsp, "max", 1024);

            return rsp;
        };
        NoteRequestResponse_fake.custom_fake = cardBinaryRspInitial;
        uint8_t originalData[sizeof(buf)];
        memcpy(originalData, buf, dataLen);

        AND_GIVEN("Allocating the card.binary.put request fails") {
            NoteNewRequest_fake.custom_fake = NULL;
            J *noteNewReqRetSequence[] = {JCreateObject(), NULL};
            SET_RETURN_SEQ(NoteNewRequest, noteNewReqRetSequence, 2);

            WHEN("NoteBinaryTransmit is called") {
                const char *err = NoteBinaryTransmit(buf, dataLen, bufLen, 0);

                THEN("An error is returned") {
                    CHECK(err != NULL);
                }

                THEN("The input buffer contains the original, unencoded data") {
                    CHECK(memcmp(buf, originalData, dataLen) == 0);
                }
            }
        }

        AND_GIVEN("The card.binary.put request fails") {
            NoteRequest_fake.custom_fake = [](J *req) -> bool {
                JDelete(req);

                return false;
            };

            WHEN("NoteBinaryTransmit is called") {
                const char *err = NoteBinaryTransmit(buf, dataLen, bufLen, 0);

                THEN("An error is returned") {
                    CHECK(err != NULL);
                }

                THEN("The input buffer contains the original, unencoded data") {
                    CHECK(memcmp(buf, originalData, dataLen) == 0);
                }
            }
        }

        AND_GIVEN("NoteChunkedTransmit fails") {
            NoteRequest_fake.custom_fake = [](J *req) -> bool {
                JDelete(req);

                return true;
            };
            NoteChunkedTransmit_fake.return_val = "some error";

            WHEN("NoteBinaryTransmit is called") {
                const char *err = NoteBinaryTransmit(buf, dataLen, bufLen, 0);

                THEN("An error is returned") {
                    CHECK(err != NULL);
                }

                THEN("The input buffer contains the original, unencoded data") {
                    CHECK(memcmp(buf, originalData, dataLen) == 0);
                }
            }
        }

        AND_GIVEN("The response to the card.binary request after the "
                  "transmission indicates a problem") {
            NoteRequest_fake.custom_fake = [](J *req) -> bool {
                JDelete(req);

                return true;
            };
            NoteChunkedTransmit_fake.return_val = NULL;

            auto initial = [](J *req) -> J * {
                JDelete(req);
                J *rsp = JCreateObject();
                JAddIntToObject(rsp, "length", 0);
                JAddIntToObject(rsp, "max", 1024);

                return rsp;
            };
            J *(*reqRespFakeSequence[])(J *) = {
                initial,
                NULL
            };
            SET_CUSTOM_FAKE_SEQ(NoteRequestResponse, reqRespFakeSequence, 2);

            AND_GIVEN("The response is NULL") {
                reqRespFakeSequence[1] = [](J *req) -> J * {
                    JDelete(req);

                    return NULL;
                };

                WHEN("NoteBinaryTransmit is called") {
                    const char *err = NoteBinaryTransmit(buf, dataLen, bufLen,
                                                         0);

                    THEN("An error is returned") {
                        CHECK(err != NULL);
                    }

                    THEN("The input buffer contains the original, unencoded "
                         "data") {
                        CHECK(memcmp(buf, originalData, dataLen) == 0);
                    }
                }
            }

            AND_GIVEN("The response has a generic error") {
                reqRespFakeSequence[1] = [](J *req) -> J * {
                    JDelete(req);
                    J *rsp = JCreateObject();
                    JAddStringToObject(rsp, "err", "some error");

                    return rsp;
                };

                WHEN("NoteBinaryTransmit is called") {
                    const char *err = NoteBinaryTransmit(buf, dataLen, bufLen,
                                                         0);

                    THEN("An error is returned") {
                        CHECK(err != NULL);
                    }

                    THEN("The input buffer contains the original, unencoded "
                         "data") {
                        CHECK(memcmp(buf, originalData, dataLen) == 0);
                    }
                }
            }

            AND_GIVEN("The response has repeated {bad-bin} errors until retries"
                      " are exhausted") {
                reqRespFakeSequence[1] = [](J *req) -> J * {
                    JDelete(req);
                    J *rsp = JCreateObject();
                    JAddStringToObject(rsp, "err", c_badbinerr);

                    return rsp;
                };

                WHEN("NoteBinaryTransmit is called") {
                    const char *err = NoteBinaryTransmit(buf, dataLen, bufLen,
                                                         0);

                    THEN("An error is returned") {
                        CHECK(err != NULL);
                    }

                    THEN("The input buffer contains the original, unencoded "
                         "data") {
                        CHECK(memcmp(buf, originalData, dataLen) == 0);
                    }
                }
            }

            AND_GIVEN("The response has a {bad-bin} error but a subsequent "
                      "response is ok") {
                auto badBin = [](J *req) -> J * {
                    JDelete(req);
                    J *rsp = JCreateObject();
                    JAddStringToObject(rsp, "err", c_badbinerr);

                    return rsp;
                };
                auto ok = [](J *req) -> J * {
                    JDelete(req);

                    return JCreateObject();
                };
                J *(*reqRespFakeSequenceSuccess[])(J *) = {
                    cardBinaryRspInitial,
                    badBin,
                    ok
                };
                SET_CUSTOM_FAKE_SEQ(NoteRequestResponse,
                                    reqRespFakeSequenceSuccess, 3);

                WHEN("NoteBinaryTransmit is called") {
                    const char *err = NoteBinaryTransmit(buf, dataLen, bufLen,
                                                         0);

                    THEN("No error is returned") {
                        CHECK(err == NULL);
                    }

                    THEN("The input buffer has been modified") {
                        CHECK(memcmp(buf, originalData, dataLen) != 0);
                    }
                }
            }
        }
    }

    THEN("The Notecard is locked and unlocked the same number of times") {
        CHECK(NoteLockNote_fake.call_count == NoteUnlockNote_fake.call_count);
    }
}

}

#endif // NOTE_C_TEST
