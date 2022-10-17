var assinatura = function assinatura() {
    "use strict";

    var fields = {};

    fields.assinar = function (pdfBase64, urlServidorAssinatura, opcaoAssinatura, successFunction, errorFunction) {
        App.blockUI({
            message: 'Carregando...',
            target: '.page-container'
        });
        hwcrypto.getCertificate({lang: 'en'}).then(function (certificate) {
            _getHash(
                hexToPem(certificate.hex),
                pdfBase64,
                urlServidorAssinatura,
                opcaoAssinatura,
                function success(response) {
                    _sign(response, certificate, urlServidorAssinatura, successFunction, errorFunction);
                },
                function error(error) {
                    errorFunction(error);
                }
            );

        }, function (err) {
            App.unblockUI('.page-container');
            errorFunction(err);
        });
    }

    fields.testCertificate = function (successFunction, errorFunction, number) {
        App.blockUI({
            message: 'Carregando...',
            target: '.page-container'
        });
        hwcrypto.getCertificate({lang: 'en'}).then(function (certificate) {
            App.unblockUI('.page-container');
            successFunction();
        }, function (err) {
            errorFunction(err);
            App.unblockUI('.page-container');
        });
    }

    fields.openGenericFile = function (result) {
        const converter = _base64ToArrayBuffer(result);
        const blob = new Blob([converter], {type: 'application/pdf'});
        const fileUrl = window.URL.createObjectURL(blob);
        window.open(fileUrl);
    }

    function _getHash(certificado, pdfBase64, urlServidorAssinatura, opcaoAssinatura, successFunction, errorFunction) {

        var data = {
            certificate: certificado,
            pdfBase64: pdfBase64,
            signaturesNumber: opcaoAssinatura
        };
        data = JSON.stringify(data);
        return $.ajax({
            type: "POST",
            url: urlServidorAssinatura + '/presign',
            headers: {
                'Referrer-Policy': 'unsafe-url',
                'Access-Control-Allow-Origin': '*',
                'Accept': 'application/json, text/plain, */*',
                'Content-Type': 'application/json'
            },
            data: data,
            dataType: 'json',
            success: successFunction,
            error: errorFunction
        });
    }

    function _sign(data, certificado, urlServidorAssinatura, successFunction, errorFunction) {
        hwcrypto.sign(certificado, {type: 'SHA-256', hex: data.shHex}, {lang: 'en'}).then(function (response) {
            data.signed = response.hex;
            $.ajax({
                type: "POST",
                url: urlServidorAssinatura + '/postsign',
                headers: {
                    'Referrer-Policy': 'unsafe-url',
                    'Access-Control-Allow-Origin': '*',
                    'Accept': 'application/json, text/plain, */*',
                    'Content-Type': 'application/json'
                },
                data: JSON.stringify(data),
                dataType: 'json',
                success: function success(response) {
                    App.unblockUI('.page-container');
                    successFunction(response);
                },
                error: function error(error) {
                    errorFunction(error);
                    App.unblockUI('.page-container');
                }
            });
        }, function (err) {
            window.alert('Erro ao pedir');

            App.unblockUI('.page-container');
        });

    }

    function downloadGenericFile(result, fileName, type) {
        const converter = _base64ToArrayBuffer(result);
        const blob = new Blob([converter], {type: type});
        const fileUrl = window.URL.createObjectURL(blob);
        const anchor = document.createElement('a');
        anchor.download = fileName;
        anchor.href = fileUrl;
        document.body.appendChild(anchor);
        anchor.click();
        document.body.removeChild(anchor);
    }



    function _base64ToArrayBuffer(base64) {
        const binaryString = atob(base64);
        const binaryLen = binaryString.length;
        const bytes = new Uint8Array(binaryLen);
        for (let i = 0; i < binaryLen; i++) {
            const ascii = binaryString.charCodeAt(i);
            bytes[i] = ascii;
        }
        return bytes;
    }


    return fields;

}();