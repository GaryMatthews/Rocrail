<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" encoding="UTF-8"/>

<xsl:template match="/">

   <html>
   <head><title>Rocrail Locomotives report</title>
  </head>
  <body>
  <h2>Compte-rendu des locomotives</h2>

        <table border="4" rules="all" cellpadding="3" cellspacing="0" width="100%">
          <THEAD>
            <TR>
              <TH width="10%" height="20" bgcolor="lightgrey">Photo</TH>
              <TH width="15%" bgcolor="lightgrey">Indicatif</TH>
              <TH width="7%" bgcolor="lightgrey">Adresse</TH>
              <TH width="7%" bgcolor="lightgrey">Longuer</TH>
              <TH width="8%" bgcolor="lightgrey">Moteur</TH>
              <TH width="11%" bgcolor="lightgrey">Type de train</TH>
              <TH width="7%" bgcolor="lightgrey">Navette</TH>
              <TH width="33%" bgcolor="lightgrey">Description</TH>

	  </TR>
      </THEAD> 
      <TBODY>
        <xsl:apply-templates/>
      </TBODY>
   </table>
   </body>
   </html>
   
</xsl:template>

<!-- triage alternative par adresse -->
<!--  
  <xsl:template match="lclist">
    <xsl:apply-templates>
      <xsl:sort select="@addr" order="ascending" data-type="number" />
    </xsl:apply-templates>
  </xsl:template>
 -->

  <xsl:variable name="engineSortOrder">steam|diesel|electric|automobile|</xsl:variable>
  <xsl:template match="lclist">
    <xsl:apply-templates>
      <xsl:sort select="@era" order="ascending" data-type="text" />
      <xsl:sort select="string-length(substring-before($engineSortOrder, @engine))" order="ascending" data-type="number"/>
      <xsl:sort select="@id" order="ascending" data-type="text" />      
    </xsl:apply-templates>
  </xsl:template>


<xsl:template match="lc">  

<!-- Wenn in der Lok-Listen Tabelle -->   
      <xsl:variable name="show1" select="@show" />    
        <xsl:choose>
          <xsl:when test="$show1 = 'true'">
    
    <TR>
      <th rowspan="3" align="center">
        <xsl:choose>
          <xsl:when test="contains(@image, '.xpm')">
            <xsl:text>-</xsl:text>
          </xsl:when>
          <xsl:when test="contains(@image, '.XPM')">
            <xsl:text>-</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <img>
              <xsl:attribute name="src">
                <xsl:value-of select="concat(../../@guiimagepath, '/', @image)" />
              </xsl:attribute>
            </img>
          </xsl:otherwise>
        </xsl:choose>
      </th>

      <TD align="center" style="color:blue;font-weight: bold;font-family:arial;">
        <xsl:value-of select="@id" />
      </TD>

      <TD align="center" style="color:blue;font-weight: bold;font-family:arial;">
        <xsl:value-of select="@addr" />
      </TD>

      <xsl:variable name="len1" select="@len" />
      <TD align="center">
        <xsl:choose>
          <xsl:when test="$len1 = ''">
            <xsl:text>0</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="@len" />
          </xsl:otherwise>
        </xsl:choose>
      </TD>

      <TD align="center">
        <xsl:variable name="engine" select="@engine" />
        <xsl:choose>
          <xsl:when test="$engine = 'none'">
            <xsl:text>-</xsl:text>
          </xsl:when>
          <xsl:when test="$engine = 'steam'">
            <xsl:text>Vapeur</xsl:text>
          </xsl:when>
          <xsl:when test="$engine = 'diesel'">
            <xsl:text>Diesel</xsl:text>
          </xsl:when>
          <xsl:when test="$engine = 'electric'">
            <xsl:text>&#201;lectrique</xsl:text>
          </xsl:when>
          <xsl:when test="$engine = 'automobile'">
            <xsl:text>Automobiles</xsl:text>
          </xsl:when>

          <xsl:otherwise>
            <xsl:value-of select="@engine" />
          </xsl:otherwise>
        </xsl:choose>
      </TD>

      <xsl:variable name="cargo" select="@cargo" />
      <TD align="center">
        <xsl:choose>
          <xsl:when test="$cargo = 'none'">
            <xsl:text>Autres</xsl:text>
          </xsl:when>
          <xsl:when test="$cargo = 'goods'">
            <xsl:text>Marchandises</xsl:text>
          </xsl:when>
          <xsl:when test="$cargo = 'person'">
            <xsl:text>Local</xsl:text>
          </xsl:when>          <xsl:when test="$cargo = 'mixed'">
            <xsl:text>Voyageurs et marchandises</xsl:text>
          </xsl:when>          <xsl:when test="$cargo = 'cleaning'">
            <xsl:text>Wagon nettoyeuer</xsl:text>
          </xsl:when>    
          <xsl:when test="$cargo = 'ice'">
            <xsl:text>Rapide</xsl:text>
          </xsl:when>
          <xsl:when test="$cargo = 'post'">
            <xsl:text>Article</xsl:text>
          </xsl:when>
          <xsl:when test="$cargo = 'light'">
            <xsl:text>&#201;clairage</xsl:text>
          </xsl:when>
          <xsl:when test="$cargo = 'lightgoods'">
            <xsl:text>Freight l&#233;ger</xsl:text>
          </xsl:when>
          <xsl:when test="$cargo = 'regional'">
            <xsl:text>R&#233;gional</xsl:text>
          </xsl:when>
          <xsl:when test="$cargo = 'all'">
            <xsl:text>Tous</xsl:text>
          </xsl:when>
          <xsl:when test="$cargo = ''">
            <xsl:text>-</xsl:text>
          </xsl:when>

          <xsl:otherwise>
            <xsl:value-of select="@cargo" />
          </xsl:otherwise>
        </xsl:choose>
      </TD>
      <xsl:variable name="commuter" select="@commuter" />
        <TD align="center">
         <xsl:choose>
         <xsl:when test="$commuter = 'false'">
          <xsl:text>-</xsl:text>
           </xsl:when>
           <xsl:when test="$commuter = 'true'">
           <xsl:text>Oui</xsl:text>
         </xsl:when>
         </xsl:choose>
      </TD>



      <TD>
        <xsl:variable name="desc1" select="@desc" />
        <xsl:choose>
          <xsl:when test="$desc1 = ''">
            <xsl:text>-</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="@desc" />
          </xsl:otherwise>
        </xsl:choose>
      </TD>

    </TR>

    <TR>
	    <td colspan="9">
        <table border="0" rules="all" cellpadding="2" cellspacing="0" width="100%">
          <THEAD>
            <TR>
              <TH width="5%" bgcolor="white">&#201;poque</TH>            
              <TH width="15%" height="5" bgcolor="white">Compagnie</TH>
              <TH width="28%" bgcolor="white">Type de d&#233;codeur</TH>
              <TH width="7%" bgcolor="white">Protokole</TH>
              <TH width="8%" bgcolor="white">Crans de vitesse</TH>                            
              <TH width="25%" bgcolor="white">Train (Longuer)</TH>
              <TH width="6%" bgcolor="white">Temps d'utilisation</TH>
              <TH width="6%" bgcolor="white">Date d'achat</TH>

            </TR>
          </THEAD>
          <TBODY>
            <TR>
             
              <xsl:variable name="era" select="@era" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="$era = 'Null'">
                    <xsl:text> - </xsl:text>
                  </xsl:when>
                  <xsl:when test="$era = '0'">
                    <xsl:text>I</xsl:text>
                  </xsl:when>
                  <xsl:when test="$era = '1'">
                    <xsl:text>II</xsl:text>
                  </xsl:when>
                  <xsl:when test="$era = '2'">
                    <xsl:text>III</xsl:text>
                  </xsl:when>
                  <xsl:when test="$era = '3'">
                    <xsl:text>IV</xsl:text>
                  </xsl:when>
                  <xsl:when test="$era = '4'">
                    <xsl:text>V</xsl:text>
                  </xsl:when>
                  <xsl:when test="$era = '5'">
                    <xsl:text>VI</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:text> - </xsl:text>
                  </xsl:otherwise>
                </xsl:choose>
                </TD>

              <xsl:variable name="roadname" select="roadname" />
                <TD align="center">
                <xsl:choose>
                  <xsl:when test="$roadname = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@roadname" />
                  </xsl:otherwise>
                </xsl:choose>
                </TD>
						   
						      <TD align="center">
	              <xsl:variable name="dectype1" select="@dectype" />
                 <xsl:choose>
                  <xsl:when test="$dectype1 = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@dectype" />
                  </xsl:otherwise>
                 </xsl:choose>
               </TD>
               
						      <TD align="center">
	              <xsl:variable name="prot1" select="@prot" />
                 <xsl:choose>
                  <xsl:when test="$prot1 = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:when test="$prot1 = 'P'">
                    <xsl:text>Server</xsl:text>
                  </xsl:when>
                  <xsl:when test="$prot1 = 'M'">
                    <xsl:text>Motorola</xsl:text>
                  </xsl:when>
                  <xsl:when test="$prot1 = 'N'">
                    <xsl:text>DCC</xsl:text>
                  </xsl:when>
                  <xsl:when test="$prot1 = 'L'">
                    <xsl:text>DCC long</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@prot" />
                  </xsl:otherwise>
                 </xsl:choose>
               </TD>

						      <TD align="center">
	              <xsl:variable name="spcnt1" select="@spcnt" />
                 <xsl:choose>
                  <xsl:when test="$spcnt1 = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@spcnt" />
                  </xsl:otherwise>
                 </xsl:choose>
               </TD>

						      <TD align="center">
	              <xsl:variable name="train1" select="@train" />
                 <xsl:choose>
                  <xsl:when test="$train1 = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@train" /> (<xsl:value-of select="@trainlen" />)
                  </xsl:otherwise>
                 </xsl:choose>
               </TD>

              <xsl:variable name="runtime" select="@runtime" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="string(number($runtime))='NaN'">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="format-number(floor(@runtime div 3600),'0')" /> h
                    <xsl:value-of select="format-number(@runtime mod 3600 div 60,'0')" /> min
                  </xsl:otherwise>
                </xsl:choose>
              </TD>

              <xsl:variable name="purchased" select="@purchased" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="$purchased = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@purchased" />
                  </xsl:otherwise>
                </xsl:choose>
              </TD>



            </TR>
          </TBODY>
        </table>
      </td>
    </TR>

    <TR>
      <td colspan="9">
        <table border="0" rules="all" cellpadding="2" cellspacing="0" width="100%">
          <THEAD>
            <TR>
              <TH width="5%" bgcolor="white">V_Min</TH>
              <TH width="5%" bgcolor="white">V_Mid</TH>
              <TH width="5%" bgcolor="white">V_Cru</TH>              
              <TH width="5%" bgcolor="white">V_Max</TH>
              <TH width="5%" bgcolor="white">V_Mode</TH>              
              <TH width="10%" bgcolor="white">D&#233;lai d'attente locomotive</TH>
              <TH width="10%" bgcolor="white">D&#233;lai d'attente du bloc</TH>
              <TH width="10%" bgcolor="white">Utiliser ShortIn</TH>
              <TH width="10%" bgcolor="white">Arr&#233;t &#224; Pre2In</TH>
            </TR>
          </THEAD>
          <TBODY>
            <TR>

              <xsl:variable name="v_min" select="@V_min" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="$v_min = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@V_min" />
                  </xsl:otherwise>
                </xsl:choose>
              </TD>

              <xsl:variable name="v_mid" select="@V_mid" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="$v_mid = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@V_mid" />
                  </xsl:otherwise>
                </xsl:choose>
              </TD>

              <xsl:variable name="v_cru" select="@V_cru" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="$v_cru = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@V_cru" />
                  </xsl:otherwise>
                </xsl:choose>
              </TD>

              <xsl:variable name="v_max" select="@V_max" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="$v_max = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@V_max" />
                  </xsl:otherwise>
                </xsl:choose>
              </TD>

              <xsl:variable name="v_mode" select="@V_mode" />
              <TD align="center">
               <xsl:choose>
                  <xsl:when test="$v_mode = 'percent'">
                    <xsl:text>%</xsl:text>
                  </xsl:when>
                  <xsl:when test="$v_mode = 'kmh'">
                    <xsl:text>Km/h</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>   
                    <xsl:value-of select="@V_mode" />                  
                  </xsl:otherwise>
                </xsl:choose>
              </TD>


              <xsl:variable name="blockwaittime" select="@blockwaittime" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="$blockwaittime = ''">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@blockwaittime" /> Sek.
                  </xsl:otherwise>
                </xsl:choose>
              </TD>

              <xsl:variable name="useownwaittime" select="@useownwaittime" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="$useownwaittime='false'">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:when test="$useownwaittime='true'">
                    <xsl:text>Oui</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@useownwaittime" /> Sek.
                  </xsl:otherwise>
                </xsl:choose>
              </TD>

              <xsl:variable name="shortin1" select="@shortin" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="$shortin1='false'">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:when test="$shortin1='true'">
                    <xsl:text>Oui</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                  <xsl:value-of select="@shortin" />
                   </xsl:otherwise>
                </xsl:choose>
                </TD>
              <xsl:variable name="inatpre2in1" select="@inatpre2in" />
              <TD align="center">
                <xsl:choose>
                  <xsl:when test="$inatpre2in1='false'">
                    <xsl:text>-</xsl:text>
                  </xsl:when>
                  <xsl:when test="$inatpre2in1='true'">
                    <xsl:text>Oui</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                  <xsl:value-of select="@inatpre2in" />
                   </xsl:otherwise>
                </xsl:choose>
                </TD>

            </TR>
          </TBODY>
        </table>
      </td>
    </TR>
    <xsl:apply-templates />

   </xsl:when>
  </xsl:choose>

  </xsl:template>
 
  
  <xsl:template match="fundef">
    <TR>
      <TD></TD>
      <TD colspan="4">
        F<xsl:value-of select="@fn" /> : <xsl:value-of select="@text" />
      </TD>
    </TR>
  </xsl:template>
  
  
  
  
</xsl:stylesheet>
