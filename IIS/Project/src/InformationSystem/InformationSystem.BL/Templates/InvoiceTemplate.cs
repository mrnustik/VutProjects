using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using InformationSystem.BL.Models.Invoice;

namespace InformationSystem.BL.Templates
{
    public class InvoiceTemplate
    {
        public string TransformText(InvoiceModel model)
        {
            return $@"
<!DOCTYPE html>
 <html lang = ""en"">
  
    <head>
  
      <meta charset = ""utf-8"">
   
       <title> Example 1 </title>
      
          <style>
      
            .clearfix:after {{
                content: "";
                display: table;
                clear: both;
            }}

            a {{
                color: #5D6975;
        text - decoration: underline;
            }}

            body {{
                position: relative;
                width: 21cm;
                height: 29.7cm;
                margin: 0 auto;
                color: #001028;
        background: #FFFFFF; 
        font - family: Arial, sans - serif;
                font - size: 12px;
                font - family: Arial;
            }}

            header {{
                padding: 10px 0;
                margin - bottom: 30px;
            }}

# logo {{
            text - align: center;
            margin - bottom: 10px;
        }}

# logo img {{
        width: 90px;
      }}

    h1 {{
        border-top: 1px solid  #5D6975;
        border-bottom: 1px solid  #5D6975;
        color: #5D6975;
        font-size: 2.4em;
        line-height: 1.4em;
        font-weight: normal;
        text-align: center;
        margin: 0 0 20px 0;
        background: url(dimension.png);
}}

# project {{
float: left;
      }}

      #project span {{
        color: #5D6975;
        text-align: right;
        width: 102px;
        margin-right: 10px;
        display: inline-block;
        font-size: 0.8em;
      }}

      #company {{
        float: right;
        text-align: right;
      }}

      #project div,
      #company div {{
        white-space: nowrap;        
      }}

      table {{
        width: 100%;
        border-collapse: collapse;
        border-spacing: 0;
        margin-bottom: 20px;
      }}

      table tr:nth-child(2n-1) td {{
        background: #F5F5F5;
      }}

      table th,
      table td {{
        text-align: center;
      }}

      table th
{{
    padding: 5px 20px;
    color: #5D6975;
        border-bottom: 1px solid #C1CED9;
        white-space: nowrap;
    font-weight: normal;
}}

table.service,
      table.desc {{
        text-align: left;
      }}

      table td
{{
    padding: 20px;
    text-align: right;
}}

table td.service,
table td.desc {{
    vertical - align: top;
}}

table td.unit,
table td.qty,
table td.total {{
    font - size: 1.2em;
}}

table td.grand
{{
    border-top: 1px solid #5D6975;;
      }}

# notices .notice {{
color: #5D6975;
        font-size: 1.2em;
      }}

      footer {{
        color: #5D6975;
        width: 100%;
        height: 30px;
        position: absolute;
        bottom: 0;
        border-top: 1px solid #C1CED9;
        padding: 8px 0;
        text-align: center;
      }}
</style>
  </head>
  <body>
    <header class=""clearfix"">
      <div id = ""logo"">
        <h1>Opravna aut</h1>
      </div>
      <h1>Faktura číslo { model.InvoiceNumber }</h1>
      <div id=""company"" class=""clearfix"">
        <div>Opravna Aut</div>
        <div>V Zahradách 10,<br /> Brno, Česká Republika</div>
        <div>(602) 519-0450</div>
        <div><a href = ""mailto:car-repairs@iis.mrnustik.net""> car-repairs@iis.mrnustik.net<a></div>
      </div>
      <div id = ""project"">
        <div><span> Projekt </span> Oprava { model.Repair.Car.CarIdenitification }</div>
        <div><span> Klient </span> { model.Owner.Name }</div>
        <div><span> Adresa </span> { model.ClientAddress }</div>
        <div><span> Email </span> <a href=""mailto:{model.Owner.Email}"">{ model.Owner.Email }</a></div>
        <div><span>Datum vydání</span> { model.Date.ToString("yyyy MMMM dd") }</div>
        <div><span>Datum splatnosti</span> { model.DueDate.ToString("yyyy MMMM dd") }</div>
        <div><span>Variabilní symbol</span> { model.InvoiceNumber }</div>
      </div>
    </header>
    <main>
      <table>
        <thead>
          <tr>
            <th class=""service"">Služba</th>
            <th class=""desc"">Popis</th>
            <th>Cena za kus</th>
            <th>Počet</th>
            <th>Celkem</th>
          </tr>
        </thead>
        <tbody>
		{string.Join("\n", model.Repair.UsedMaterials.Select(
           material =>
           $@"<tr>
            <td class=""service"">Použitý materiál</td>
			<td class=""desc"">{material.Material.Name}</td>
            <td class=""unit"">{material.Material.Price} Kč</td>
            <td class=""qty"">{material.Ammount}</td>
            <td class=""total"">{material.Ammount * material.Material.Price} Kč</td>
          </tr>"
           ))}
          <tr>
            <td class=""service"">Práce</td>
            <td class=""desc"">Práce</td>
            <td class=""unit"">200.00 Kč</td>
            <td class=""qty"">{model.Repair.Duration}h</td>
            <td class=""total"">{200 * model.Repair.Duration } Kč</td>
          </tr>
          <tr>
            <td colspan = ""4""> Cena bez DPH</td>
            <td class=""total"">{model.TotalPrice} Kč</td>
          </tr>
          <tr>
            <td colspan = ""4""> DPH 21%</td>
            <td class=""total"">${ model.TaxPrice } Kč</td>
          </tr>
          <tr>
            <td colspan = ""4"" class=""grand total"">Cena s DPH</td>
              <td class=""grand total"">${ model.TotalPriceWithTax }</td>
          </tr>
        </tbody>
      </table>
      <div id = ""notices"">
      </div>
    </main>
    <footer>
      Tato faktura byla vytvořena v rámci našeho systému a je splatná hned po datu vydání.
    </footer>
  </body>
</html>";

        }

    }
}
