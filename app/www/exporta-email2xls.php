<?php




if (!defined('APPRAIZ')) {
    define('APPRAIZ', realpath('../') . "/");
}

require_once APPRAIZ . 'global/config.inc';
require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'seguranca/modulos/sistema/geral/endereco.inc';



$sql = "
        select
            e.emaid         as a,
            ed.usucpf       as b,
            u.usunome       as c,
            un.unidsc       as d,
            e.usucpf        as e,
            us.usunome      as f,
            un2.unidsc      as g,
            e.emaconteudo   as h,
            to_char(e.emadata, 'DD/MM/YYYY HH12:MI:SS') as emadata
        from
            email e
        inner join seguranca.usuario us ON us.usucpf = e.usucpf
        inner join emaildestinatario ed ON ed.emaid = e.emaid
        inner join seguranca.usuario u ON u.usucpf = ed.usucpf
        inner join public.unidade un ON un.unicod = u.unicod
        inner join public.unidade un2 ON un2.unicod = us.unicod
        where
            e.usucpf <> '64756050182' and un2.unicod = '26101'
        group by
            e.emaid,
            ed.usucpf,
            u.usunome,
            un.unidsc,
            e.usucpf,
            us.usunome,
            un2.unidsc,
            e.emaconteudo,
            e.emadata
        order by
            e.emaid, e.emadata,
            ed.usucpf,
            u.usunome,
            e.usucpf,
            us.usunome,
            e.emaconteudo";



$db    = new cls_banco();
$dados = (array) $db->carregar($sql);

header("Content-type: application/octet-stream");
header("Content-Disposition: attachment; filename=email.xls");
header("Pragma: no-cache");
header("Expires: 0");


echo "<table border=\"1\"><tr>
<td>emaid</td>
<td>usucpf</td>
<td>usunome</td>
<td>unidsc</td>
<td>usucpf</td>
<td>usunome</td>
<td>unidsc</td>
<td>emaconteudo</td>
<td>emadata</td></tr>";

foreach ($dados as $linha) {
    echo "<tr>
    <td style=\"white-space: nowrap\">{$linha['a']}</td>
    <td style=\"white-space: nowrap\">{$linha['b']}</td>
    <td style=\"white-space: nowrap\">{$linha['c']}</td>
    <td style=\"white-space: nowrap\">{$linha['d']}</td>
    <td style=\"white-space: nowrap\">{$linha['e']}</td>
    <td style=\"white-space: nowrap\">{$linha['f']}</td>
    <td style=\"white-space: nowrap\">{$linha['g']}</td>
    <td style=\"white-space: nowrap\">{$linha['h']}</td>
    <td style=\"white-space: nowrap\">{$linha['emadata']}</td>
</tr>";
}


echo '</table>';




