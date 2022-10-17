function adicionarLabelObrigatorio(elems){
    adicionarObrigatorio(elems.closest('div.form-group').find('label:not(.not-obrigatorio)'));
}

function adicionarObrigatorio(elems){
    elems.addClass('bold').append(" <i class='fa fa-asterisk' style='color: red; font-size: 6px; vertical-align: text-top;' ></i>");
}

if(window.jQuery){
    $(function(){
        $('[data-toggle="tooltip"]').tooltip();

        if($().selectpicker){
            $('.bs-select').selectpicker({
                iconBase: 'fa',
                tickIcon: 'fa-check'
            });
        }

        adicionarDatePicker();

        // Destaca os campos required
        adicionarLabelObrigatorio($(':required'));
        adicionarObrigatorio($('.obrigatorio'));

        $('.btn-pesquisa-avancada, .btn-pesquisa-simples').click(function(){
            gerenciarPesquisas(this);
        });

        adicionarAcaoBtnAnexoDocumentos();

        $(".portlet-body > form ").trackChanges();
    });

    // Verifica mudanças no form
    $.fn.extend({
        trackChanges: function() {
            if(!$(this).hasClass('dont-check-changes')) {
                $(":input, select, textarea", this).change(function() {
                    $(this.form).data("changed", true);
                });

                $(this).submit(function(){
                    $(this).data("submitted", true);
                });
            }
        },
        isChanged: function() {
            return this.data("changed") ? this.data("changed") : false;
        },
        isSubmitted: function() {
            return this.data("submitted") ? this.data("submitted") : false;
        }
    });
}

window.addEventListener("beforeunload", function (e) {
    if($('[name=formulario]').isChanged() && !$('[name=formulario]').isSubmitted()){
        var confirmationMessage = "Existem alterações não salvas, deseja sair?";

        (e || window.event).returnValue = confirmationMessage; //Gecko + IE
        return confirmationMessage;                            //Webkit, Safari, Chrome
    }

    return true;
});

/*
 * Date Format 1.2.3
 * (c) 2007-2009 Steven Levithan <stevenlevithan.com>
 * MIT license
 *
 * Includes enhancements by Scott Trenda <scott.trenda.net>
 * and Kris Kowal <cixar.com/~kris.kowal/>
 *
 * Accepts a date, a mask, or a date and a mask.
 * Returns a formatted version of the given date.
 * The date defaults to the current date/time.
 * The mask defaults to dateFormat.masks.default.
 */

var dateFormat = function () {
    var	token = /d{1,4}|m{1,4}|yy(?:yy)?|([HhMsTt])\1?|[LloSZ]|"[^"]*"|'[^']*'/g,
        timezone = /\b(?:[PMCEA][SDP]T|(?:Pacific|Mountain|Central|Eastern|Atlantic) (?:Standard|Daylight|Prevailing) Time|(?:GMT|UTC)(?:[-+]\d{4})?)\b/g,
        timezoneClip = /[^-+\dA-Z]/g,
        pad = function (val, len) {
            val = String(val);
            len = len || 2;
            while (val.length < len) val = "0" + val;
            return val;
        };

    // Regexes and supporting functions are cached through closure
    return function (date, mask, utc) {
        var dF = dateFormat;

        // You can't provide utc if you skip other args (use the "UTC:" mask prefix)
        if (arguments.length == 1 && Object.prototype.toString.call(date) == "[object String]" && !/\d/.test(date)) {
            mask = date;
            date = undefined;
        }

        // Passing date through Date applies Date.parse, if necessary
        date = date ? new Date(date) : new Date;
        if (isNaN(date)) throw SyntaxError("invalid date");

        mask = String(dF.masks[mask] || mask || dF.masks["default"]);

        // Allow setting the utc argument via the mask
        if (mask.slice(0, 4) == "UTC:") {
            mask = mask.slice(4);
            utc = true;
        }

        var	_ = utc ? "getUTC" : "get",
            d = date[_ + "Date"](),
            D = date[_ + "Day"](),
            m = date[_ + "Month"](),
            y = date[_ + "FullYear"](),
            H = date[_ + "Hours"](),
            M = date[_ + "Minutes"](),
            s = date[_ + "Seconds"](),
            L = date[_ + "Milliseconds"](),
            o = utc ? 0 : date.getTimezoneOffset(),
            flags = {
                d:    d,
                dd:   pad(d),
                ddd:  dF.i18n.dayNames[D],
                dddd: dF.i18n.dayNames[D + 7],
                m:    m + 1,
                mm:   pad(m + 1),
                mmm:  dF.i18n.monthNames[m],
                mmmm: dF.i18n.monthNames[m + 12],
                yy:   String(y).slice(2),
                yyyy: y,
                h:    H % 12 || 12,
                hh:   pad(H % 12 || 12),
                H:    H,
                HH:   pad(H),
                M:    M,
                MM:   pad(M),
                s:    s,
                ss:   pad(s),
                l:    pad(L, 3),
                L:    pad(L > 99 ? Math.round(L / 10) : L),
                t:    H < 12 ? "a"  : "p",
                tt:   H < 12 ? "am" : "pm",
                T:    H < 12 ? "A"  : "P",
                TT:   H < 12 ? "AM" : "PM",
                Z:    utc ? "UTC" : (String(date).match(timezone) || [""]).pop().replace(timezoneClip, ""),
                o:    (o > 0 ? "-" : "+") + pad(Math.floor(Math.abs(o) / 60) * 100 + Math.abs(o) % 60, 4),
                S:    ["th", "st", "nd", "rd"][d % 10 > 3 ? 0 : (d % 100 - d % 10 != 10) * d % 10]
            };

        return mask.replace(token, function ($0) {
            return $0 in flags ? flags[$0] : $0.slice(1, $0.length - 1);
        });
    };
}();

// Some common format strings
dateFormat.masks = {
    "default":      "ddd mmm dd yyyy HH:MM:ss",
    shortDate:      "m/d/yy",
    mediumDate:     "mmm d, yyyy",
    longDate:       "mmmm d, yyyy",
    fullDate:       "dddd, mmmm d, yyyy",
    shortTime:      "h:MM TT",
    mediumTime:     "h:MM:ss TT",
    longTime:       "h:MM:ss TT Z",
    isoDate:        "yyyy-mm-dd",
    isoTime:        "HH:MM:ss",
    isoDateTime:    "yyyy-mm-dd'T'HH:MM:ss",
    isoUtcDateTime: "UTC:yyyy-mm-dd'T'HH:MM:ss'Z'"
};

// Internationalization strings
dateFormat.i18n = {
    dayNames: [
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat",
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
    ],
    monthNames: [
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
        "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
    ]
};

//! moment.js locale configuration
//! locale : Portuguese (Brazil) [pt-br]
//! author : Caio Ribeiro Pereira : https://github.com/caio-ribeiro-pereira

;(function (global, factory) {
    typeof exports === 'object' && typeof module !== 'undefined'
    && typeof require === 'function' ? factory(require('../moment')) :
        typeof define === 'function' && define.amd ? define(['../moment'], factory) :
            factory(global.moment)
}(this, (function (moment) { 'use strict';


    var ptBr = moment.defineLocale('pt-br', {
        months : 'Janeiro_Fevereiro_Março_Abril_Maio_Junho_Julho_Agosto_Setembro_Outubro_Novembro_Dezembro'.split('_'),
        monthsShort : 'Jan_Fev_Mar_Abr_Mai_Jun_Jul_Ago_Set_Out_Nov_Dez'.split('_'),
        weekdays : 'Domingo_Segunda-feira_Terça-feira_Quarta-feira_Quinta-feira_Sexta-feira_Sábado'.split('_'),
        weekdaysShort : 'Dom_Seg_Ter_Qua_Qui_Sex_Sáb'.split('_'),
        weekdaysMin : 'Do_2ª_3ª_4ª_5ª_6ª_Sá'.split('_'),
        weekdaysParseExact : true,
        longDateFormat : {
            LT : 'HH:mm',
            LTS : 'HH:mm:ss',
            L : 'DD/MM/YYYY',
            LL : 'D [de] MMMM [de] YYYY',
            LLL : 'D [de] MMMM [de] YYYY [às] HH:mm',
            LLLL : 'dddd, D [de] MMMM [de] YYYY [às] HH:mm'
        },
        calendar : {
            sameDay: '[Hoje às] LT',
            nextDay: '[Amanhã às] LT',
            nextWeek: 'dddd [às] LT',
            lastDay: '[Ontem às] LT',
            lastWeek: function () {
                return (this.day() === 0 || this.day() === 6) ?
                    '[Último] dddd [às] LT' : // Saturday + Sunday
                    '[Última] dddd [às] LT'; // Monday - Friday
            },
            sameElse: 'L'
        },
        relativeTime : {
            future : 'em %s',
            past : '%s atrás',
            s : 'poucos segundos',
            m : 'um minuto',
            mm : '%d minutos',
            h : 'uma hora',
            hh : '%d horas',
            d : 'um dia',
            dd : '%d dias',
            M : 'um mês',
            MM : '%d meses',
            y : 'um ano',
            yy : '%d anos'
        },
        dayOfMonthOrdinalParse: /\d{1,2}º/,
        ordinal : '%dº'
    });

    return ptBr;

})));

// For convenience...
Date.prototype.format = function (mask, utc) {
    return dateFormat(this, mask, utc);
};

function formatarDataHora(value){
    return value ? new Date(value).format("dd/mm/yyyy HH:MM:ss") : '';
}

function formatarData(value){
    if(!value){
        return '';
    }

    if(value.indexOf(':') === -1){
        value += ' 00:00:00';
    }

    return new Date(value).format("dd/mm/yyyy");
}

function limparPesquisa(){
    window.location = window.location.href;
}

function adicionarItemTabela(item, table, index) {
    var dados = table.bootstrapTable("getData");
    index = parseInt(index);

    if(Number.isInteger(index) && dados[index]){
        item.id = dados[index].id;
        dados[index] = Object.assign(dados[index], item);
    }else{
        dados.push(item);
    }

    table.bootstrapTable("load", dados);

    $(table).closest('form').data("changed", true);
}

function getItemTabela(index, table) {
    var dados = table.bootstrapTable("getData");
    return dados[index];
}

function removerItemTabela(index, table) {
    var dados = table.bootstrapTable("getData");

    dados.splice(index, 1);
    table.bootstrapTable("load", dados);

    $(table).closest('form').data("changed", true);
}

function atualizarTabela(table){
    table.bootstrapTable("load", table.bootstrapTable('getData'));
}

$.fn.validateForm = function (options) {
    var form = $(this).prop('tagName') == 'FORM' ? $(this) : $(this).closest('form');
    if (form) {
        var defaults = {
            rules: {},
            messages: {},
            invalidHandler: function (e, r) {},
            highlight: function (e) {
                $(e).closest(".form-group").addClass("has-error");
            },
            success: function (e) {
                e.closest(".form-group").removeClass("has-error");
                e.remove();
            },
            errorPlacement: function (e, r) {
                this.errorPlacementDefault(e,r);
            },
            errorPlacementDefault: function (e, r) {
                if(r.closest(".has-error").find('.help-block').size() !== 0){
                    return;
                }

                switch (true) {
                    case 1 === r.closest(".fileinput").size() :
                        e.insertAfter(r.closest(".fileinput"));
                        break;
                    case 1 === r.closest(".mt-radio-inline").size() :
                        e.insertAfter(r.closest(".mt-radio-inline"));
                        break;
                    case 1 === r.closest(".input-icon").size() :
                        e.insertAfter(r.closest(".input-icon"));
                        break;
                    case 1 === r.closest(".input-group").size() :
                        e.insertAfter(r.closest(".input-group"));
                        break;
                    case 1 === r.closest(".selectized").size() :
                        e.insertAfter(r.parent().find(".selectize-input"));
                        break;
                    case 1 === r.closest(".input-container").size() :
                        e.insertAfter(r.closest(".input-container"));
                        break;
                    case 1 === r.closest(".bootstrap-select").size() :
                        e.insertAfter(r.closest(".bootstrap-select"));
                        break;
                    default :
                        e.insertAfter(r);
                        break;
                }
            },
            submitHandler: function (e) {
                this.submitForm(e);
            },
            beforeSubmitForm: function (e) {},
            submitHandlerFile: function (e, inputFile) {
                var minWidth = 250, minHeight = 250, jFileInput = $(inputFile),
                    fileInput = jFileInput[0],
                    file = fileInput.files && fileInput.files[0];

                if (file) {
                    var img = new Image();
                    var settings = this;
                    img.src = window.URL.createObjectURL(file);

                    img.onload = function () {
                        var width = img.naturalWidth,
                            height = img.naturalHeight;
                        window.URL.revokeObjectURL(img.src);

                        if (width >= minWidth && height >= minHeight) {
                            settings.submitForm(e);
                        } else {
                            addMsgErroCampo($(jFileInput).closest('div.fileinput'), 'Por favor, selecione uma imagem com no mínimo 250x250');
                        }
                    };
                } else {
                    this.submitForm(e);
                }
            },
            submitForm: function (e) {
                this.beforeSubmitForm(e);
                $(e).find('input[type=submit], button[type=submit]').prop('disabled', true).html('Enviando...');
                e.submit();
            }
        };

        var settings = $.extend({}, defaults, options);

        $(form).validate({
            errorElement: "span",
            errorClass: "help-block",
            focusInvalid: true,
            ignore: "",
            rules: settings.rules,
            messages: settings.messages,
            invalidHandler: function (e, r) {
                settings.invalidHandler(e, r);
            },
            highlight: function (e) {
                settings.highlight(e);
            },
            success: function (e) {
                settings.success(e);
            },
            errorPlacement: function (e, r) {
                settings.errorPlacement(e, r);
            },
            submitHandler: function (e) {
                settings.submitHandler(e);
            },
        });

        $(form).find("input").keypress(function (e) {
            return 13 == e.which ? ($(form).validate().form() && settings.submitHandler($(form)), false) : void 0;
        });

    } else {
        console.error("Form não encontrado");
    }
};

function adicionarDatePicker(elem){
    elem = elem ? elem : $('body');
    elem.find('.date-picker').datepicker({
        orientation: "left",
        autoclose: true,
        language: 'pt-BR'
    });
}

function exibirMsg(msg, tipo){
    $.bootstrapGrowl(msg, {
        ele: 'body', // which element to append to
        type: tipo, // (null, 'info', 'danger', 'success', 'warning')
        offset: {
            from: 'top',
            amount: 60
        }, // 'top', or 'bottom'
        align: 'center', // ('left', 'right', or 'center')
        width: 800, // (integer, or 'auto')
        delay: 5000, // Time while the message will be displayed. It's not equivalent to the *demo* timeOut!
        allow_dismiss: true, // If true then will display a cross to close the popup.
        stackup_spacing: 10 // spacing between consecutively stacked growls.
    });
}

function exibirAlert(msg){
    exibirMsg(msg, 'danger');
}

function exibirAviso(msg){
    exibirMsg(msg, 'warning');
}

function exibirSucesso(msg){
    exibirMsg(msg, 'success');
}

function gerenciarPesquisas(button){
    var container = $(button).closest('form');

    if(container.find('.pesquisa-simples:visible').length > 0){
        container.find('.pesquisa-simples, .btn-pesquisa-avancada').addClass('hidden');
        container.find('.pesquisa-avancada, .btn-pesquisa-simples').removeClass('hidden');
        container.find('input[type=hidden][name=act]').val('pesquisa_avancada');
    }else{
        container.find('.pesquisa-avancada, .btn-pesquisa-simples').addClass('hidden');
        container.find('.pesquisa-simples, .btn-pesquisa-avancada').removeClass('hidden');
        container.find('input[type=hidden][name=act]').val('pesquisa_simples');
    }
}

function adicionarAcaoBtnAnexoDocumentos() {
    $('.btn-anexo-documento').popover({
        html: true,
        placement : 'top',
        title : 'Enviar arquivo',
        container: 'body',
        // trigger: 'focus',
        content: function() {
            var tipo = $(this).data('type');

            var arqid = $('[name=arquivo_'+tipo+']').val();
            var htmlDownload = '';
            if(arqid){
                htmlDownload = '<div class="input-group text-center" style="padding-top: 5px; width: 100%;">' +
                    '    <a href="javascript:;" onclick="downloadArquivoDocumento(' + arqid + ')">' +
                    '        <button class="btn btn-icon-only btn-default green btn-circle" type="button" title="Cancelar">' +
                    '            <i class="icon-cloud-download"></i>' +
                    '        </button>' +
                    '        Download' +
                    '    </a>' +
                    '</div>';
            }

            return '<form name="form-anexo-'+tipo+'" method="post" enctype="multipart/form-data" class="form">' +
                '    <input type="hidden" name="act" value="enviarArquivoDocumento" />' +
                '    <input type="hidden" name="tipo" value="'+tipo+'" />' +
                '    <div class="input-group" style="width: 246px;">' +
                '        <input type="file" id="documento" name="documento" class="normal form-control" required />' +
                '        <span class="input-group-btn">' +
                '            <button class="btn btn-icon-only btn-default blue btn-enviar-anexo-'+tipo+'" type="submit" title="Enviar">' +
                '                <i class="icon-cloud-upload"></i>' +
                '            </button>' +
                '            <button class="btn btn-icon-only btn-default red" type="button" title="Cancelar" onclick="$(\'.btn-anexo-'+tipo+'\').click();">' +
                '                <i class="icon-close"></i>' +
                '            </button>' +
                '        </span>' +
                '    </div>' +
                '    ' + htmlDownload +
                '</form>';
        }
    }).click(function(){
        var tipo = $(this).data('type');

        $("[name=form-anexo-"+tipo+"]").validateForm({
            rules : {},
            submitHandler: function (e) {
                $('.btn-anexo-'+tipo+'').click();

                App.blockUI({
                    message: 'Carregando...',
                    target: '.form-body'
                });

                var form = $("[name=form-anexo-"+tipo+"]")[0];
                var formData = new FormData(form);

                $.ajax({
                    url: window.location.href,
                    type: 'POST',
                    data: formData,
                    dataType : 'json',
                    success: function (data) {
                        $('[name=arquivo_'+tipo+']').val(data.params.arqid);
                        exibirSucesso('Arquivo enviado');
                        App.unblockUI('.form-body');
                    },
                    error: function (data) {
                        exibirAlert('Não foi possível enviar o arquivo');
                        App.unblockUI('.form-body');
                    },
                    cache: false,
                    contentType: false,
                    processData: false,
                    xhr: function() {  // Custom XMLHttpRequest
                        var myXhr = $.ajaxSettings.xhr();
                        if (myXhr.upload) { // Avalia se tem suporte a propriedade upload
                            myXhr.upload.addEventListener('progress', function () {
                                /* faz alguma coisa durante o progresso do upload */
                            }, false);
                        }

                        return myXhr;
                    }
                });
            }
        });
    });
}

function downloadArquivoDocumento(arqid){
    window.location = window.location.href + "&act=downloadArquivoDocumento&arqid=" + arqid;
}

function abrirUsuariosOnline()
{
    window.open(
        '../geral/usuarios_online.php',
        'usuariosonline',
        'height=500,width=600,scrollbars=yes,top=50,left=200'
    );
}

function valueToFloat (value) {
    if(!value) {
        return 0;
    }

    return parseFloat(value.replace(/[.,]/g, function (match) {
        return match === '.' ? '' : '.';
    }));
}
