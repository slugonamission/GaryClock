<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="con-tycoelectronics">
<description>&lt;b&gt;Tyco Electronics Connector&lt;/b&gt;&lt;p&gt;
http://catalog.tycoelectronics.com&lt;br&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="6ESRM-P">
<description>&lt;b&gt;AC Connector&lt;/b&gt;&lt;p&gt;
120/250 VAC&lt;br&gt;
Source: http://catalog.tycoelectronics.com .. ENG_DS_1654001_1099_SR_0406.pdf</description>
<wire x1="-24.9" y1="-16.9" x2="24.9" y2="-16.9" width="0.2032" layer="21"/>
<wire x1="24.9" y1="-16.9" x2="24.9" y2="-13.9" width="0.2032" layer="21"/>
<wire x1="24.9" y1="-13.9" x2="-24.9" y2="-13.9" width="0.2032" layer="21"/>
<wire x1="-24.9" y1="-13.9" x2="-24.9" y2="-16.9" width="0.2032" layer="21"/>
<wire x1="-15.025" y1="-16.95" x2="-15.025" y2="-18.475" width="0.2032" layer="21"/>
<wire x1="-15.025" y1="-18.475" x2="15.025" y2="-18.475" width="0.2032" layer="21"/>
<wire x1="15.025" y1="-18.475" x2="15.025" y2="-16.975" width="0.2032" layer="21"/>
<wire x1="-24.9" y1="-13.9" x2="-24.9" y2="6.4" width="0.2032" layer="21"/>
<wire x1="-24.9" y1="6.4" x2="-14.605" y2="6.4" width="0.2032" layer="21"/>
<wire x1="14.605" y1="6.4" x2="24.9" y2="6.4" width="0.2032" layer="21"/>
<wire x1="24.9" y1="6.4" x2="24.9" y2="-13.9" width="0.2032" layer="21"/>
<wire x1="-14.605" y1="8.255" x2="14.605" y2="8.255" width="0.2032" layer="21"/>
<wire x1="14.605" y1="8.255" x2="14.605" y2="6.4" width="0.2032" layer="21"/>
<wire x1="14.605" y1="6.4" x2="14.605" y2="-13.97" width="0.2032" layer="21"/>
<wire x1="-14.605" y1="8.255" x2="-14.605" y2="6.4" width="0.2032" layer="21"/>
<wire x1="-14.605" y1="6.4" x2="-14.605" y2="-13.97" width="0.2032" layer="21"/>
<pad name="1" x="-9.1" y="9.7" drill="1.6" diameter="2.6"/>
<pad name="2" x="0" y="9.7" drill="1.6" diameter="2.6"/>
<pad name="3" x="9.1" y="9.7" drill="1.6" diameter="2.6"/>
<text x="-23.495" y="6.985" size="1.27" layer="25">&gt;NAME</text>
<text x="-10.16" y="-8.255" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-9.675" y1="8.25" x2="-8.525" y2="10.175" layer="51"/>
<rectangle x1="-0.575" y1="8.25" x2="0.575" y2="10.175" layer="51"/>
<rectangle x1="8.525" y1="8.25" x2="9.675" y2="10.175" layer="51"/>
<hole x="-20" y="0" drill="3.6"/>
<hole x="20" y="0" drill="3.6"/>
</package>
</packages>
<symbols>
<symbol name="AC-CONNECTOR">
<wire x1="0" y1="6.35" x2="0" y2="-6.35" width="0.254" layer="94"/>
<wire x1="0" y1="-6.35" x2="8.89" y2="-6.35" width="0.254" layer="94"/>
<wire x1="8.89" y1="-6.35" x2="8.89" y2="6.35" width="0.254" layer="94"/>
<wire x1="8.89" y1="6.35" x2="0" y2="6.35" width="0.254" layer="94"/>
<wire x1="4.8696" y1="-5.08" x2="5.2288" y2="-4.9312" width="0.254" layer="94" curve="44.999323"/>
<wire x1="5.2288" y1="-4.9312" x2="7.4712" y2="-2.6888" width="0.254" layer="94"/>
<wire x1="7.4712" y1="-2.6888" x2="7.62" y2="-2.3296" width="0.254" layer="94" curve="44.999323"/>
<wire x1="7.62" y1="-2.3296" x2="7.62" y2="2.3296" width="0.254" layer="94"/>
<wire x1="7.62" y1="2.3296" x2="7.4712" y2="2.6888" width="0.254" layer="94" curve="44.999323"/>
<wire x1="7.4712" y1="2.6888" x2="5.2288" y2="4.9312" width="0.254" layer="94"/>
<wire x1="5.2288" y1="4.9312" x2="4.8696" y2="5.08" width="0.254" layer="94" curve="44.999323"/>
<wire x1="4.8696" y1="5.08" x2="1.778" y2="5.08" width="0.254" layer="94"/>
<wire x1="1.778" y1="5.08" x2="1.27" y2="4.572" width="0.254" layer="94" curve="90"/>
<wire x1="1.27" y1="4.572" x2="1.27" y2="-4.572" width="0.254" layer="94"/>
<wire x1="1.27" y1="-4.572" x2="1.778" y2="-5.08" width="0.254" layer="94" curve="90"/>
<wire x1="1.778" y1="-5.08" x2="4.8696" y2="-5.08" width="0.254" layer="94"/>
<rectangle x1="2.54" y1="3.429" x2="4.572" y2="4.064" layer="94"/>
<rectangle x1="5.08" y1="-0.254" x2="6.985" y2="0.254" layer="94"/>
<rectangle x1="2.54" y1="-3.937" x2="4.572" y2="-3.302" layer="94"/>
<pin name="AC1" x="-2.54" y="5.08" visible="pad" length="short" direction="pas"/>
<pin name="AC2" x="-2.54" y="-5.08" visible="pad" length="short" direction="pas"/>
<pin name="SH" x="-2.54" y="0" visible="pad" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="6ESRM-P" prefix="X">
<description>&lt;b&gt;AC Connector&lt;/b&gt;&lt;p&gt;
120/250 VAC&lt;br&gt;
Source: http://catalog.tycoelectronics.com .. ENG_DS_1654001_1099_SR_0406.pdf</description>
<gates>
<gate name="G$1" symbol="AC-CONNECTOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="6ESRM-P">
<connects>
<connect gate="G$1" pin="AC1" pad="1"/>
<connect gate="G$1" pin="AC2" pad="3"/>
<connect gate="G$1" pin="SH" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="TYCO ELECTRONICS" constant="no"/>
<attribute name="MPN" value="6ESRM-P" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="87F1821" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="con-berg">
<description>&lt;b&gt;Berg Connectors&lt;/b&gt;&lt;p&gt;
Based on  the following source:
&lt;ul&gt;
&lt;li&gt;http://catalog.fciconnect.com
&lt;/ul&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="PN87520">
<description>&lt;b&gt;USB connector&lt;/b&gt;</description>
<wire x1="-7.4" y1="-10.19" x2="7.4" y2="-10.19" width="0.254" layer="21"/>
<wire x1="7.4" y1="-10.19" x2="7.4" y2="4.11" width="0.254" layer="21"/>
<wire x1="7.4" y1="4.11" x2="-7.4" y2="4.11" width="0.254" layer="21"/>
<wire x1="-7.4" y1="4.11" x2="-7.4" y2="-10.19" width="0.254" layer="21"/>
<wire x1="-5.08" y1="-2.87" x2="-3.81" y2="-8.72" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-8.72" x2="-2.54" y2="-8.72" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-8.72" x2="-1.27" y2="-2.87" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-2.87" x2="2.54" y2="-8.72" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-8.72" x2="3.81" y2="-8.72" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-8.72" x2="5.08" y2="-2.87" width="0.1524" layer="21"/>
<wire x1="-2.46" y1="-0.1" x2="-2.46" y2="0.9" width="0.0508" layer="21" curve="180"/>
<wire x1="-2.46" y1="-1.1" x2="-2.46" y2="-0.1" width="0.0508" layer="21" curve="180"/>
<wire x1="3.665" y1="0.4" x2="3.665" y2="-0.6" width="0.0508" layer="21" curve="180"/>
<wire x1="3.415" y1="0.9" x2="3.415" y2="-1.1" width="0.0508" layer="21" curve="180"/>
<wire x1="3.665" y1="0.4" x2="4.165" y2="0.4" width="0.0508" layer="21" curve="-15.189287"/>
<wire x1="3.415" y1="0.9" x2="4.175" y2="0.845" width="0.0508" layer="21" curve="-12.05913"/>
<wire x1="3.415" y1="-1.1" x2="4.165" y2="-0.975" width="0.0508" layer="21" curve="18.422836"/>
<wire x1="1.665" y1="-0.35" x2="1.665" y2="0.9" width="0.0508" layer="21" curve="180"/>
<wire x1="1.29" y1="0.025" x2="1.29" y2="0.4" width="0.0508" layer="21" curve="180"/>
<wire x1="-3.835" y1="0.9" x2="-3.835" y2="-1.1" width="0.0508" layer="21"/>
<wire x1="-3.835" y1="-1.1" x2="-3.21" y2="-1.1" width="0.0508" layer="21"/>
<wire x1="-3.21" y1="-1.1" x2="-3.21" y2="-0.1" width="0.0508" layer="21"/>
<wire x1="-3.21" y1="-0.1" x2="-3.21" y2="0.9" width="0.0508" layer="21"/>
<wire x1="-3.21" y1="0.9" x2="-3.835" y2="0.9" width="0.0508" layer="21"/>
<wire x1="-3.21" y1="0.9" x2="-2.46" y2="0.4" width="0.0508" layer="21"/>
<wire x1="-2.46" y1="0.4" x2="-3.21" y2="-0.1" width="0.0508" layer="21"/>
<wire x1="-3.21" y1="-0.1" x2="-2.46" y2="-0.6" width="0.0508" layer="21"/>
<wire x1="-2.46" y1="-0.6" x2="-3.21" y2="-1.1" width="0.0508" layer="21"/>
<wire x1="-2.46" y1="-1.1" x2="-3.21" y2="-1.1" width="0.0508" layer="21"/>
<wire x1="-2.46" y1="0.9" x2="-3.21" y2="0.9" width="0.0508" layer="21"/>
<wire x1="-1.71" y1="0.9" x2="-1.71" y2="-1.1" width="0.0508" layer="21"/>
<wire x1="-1.71" y1="-1.1" x2="0.04" y2="-1.1" width="0.0508" layer="21"/>
<wire x1="0.04" y1="-1.1" x2="0.04" y2="-0.6" width="0.0508" layer="21"/>
<wire x1="0.04" y1="-0.6" x2="-1.085" y2="-0.6" width="0.0508" layer="21"/>
<wire x1="-1.085" y1="-0.6" x2="-1.085" y2="-0.35" width="0.0508" layer="21"/>
<wire x1="-1.085" y1="-0.35" x2="0.04" y2="-0.35" width="0.0508" layer="21"/>
<wire x1="0.04" y1="-0.35" x2="0.04" y2="0.15" width="0.0508" layer="21"/>
<wire x1="0.04" y1="0.15" x2="-1.085" y2="0.15" width="0.0508" layer="21"/>
<wire x1="-1.085" y1="0.15" x2="-1.085" y2="0.4" width="0.0508" layer="21"/>
<wire x1="-1.085" y1="0.4" x2="0.04" y2="0.4" width="0.0508" layer="21"/>
<wire x1="0.04" y1="0.4" x2="0.04" y2="0.9" width="0.0508" layer="21"/>
<wire x1="0.04" y1="0.9" x2="-1.71" y2="0.9" width="0.0508" layer="21"/>
<wire x1="0.29" y1="0.9" x2="0.29" y2="-1.1" width="0.0508" layer="21"/>
<wire x1="0.29" y1="-1.1" x2="0.915" y2="-1.1" width="0.0508" layer="21"/>
<wire x1="0.915" y1="-1.1" x2="0.915" y2="-0.35" width="0.0508" layer="21"/>
<wire x1="0.915" y1="-0.35" x2="1.415" y2="-1.1" width="0.0508" layer="21"/>
<wire x1="1.415" y1="-1.1" x2="2.165" y2="-1.1" width="0.0508" layer="21"/>
<wire x1="2.165" y1="-1.1" x2="1.665" y2="-0.35" width="0.0508" layer="21"/>
<wire x1="0.915" y1="0.4" x2="0.915" y2="0.025" width="0.0508" layer="21"/>
<wire x1="0.29" y1="0.9" x2="1.665" y2="0.9" width="0.0508" layer="21"/>
<wire x1="0.915" y1="0.4" x2="1.29" y2="0.4" width="0.0508" layer="21"/>
<wire x1="0.915" y1="0.025" x2="1.29" y2="0.025" width="0.0508" layer="21"/>
<wire x1="3.665" y1="-0.1" x2="4.165" y2="-0.1" width="0.0508" layer="21"/>
<wire x1="3.665" y1="-0.1" x2="3.665" y2="-0.6" width="0.0508" layer="21"/>
<wire x1="4.16" y1="0.4" x2="4.16" y2="0.845" width="0.0508" layer="21"/>
<wire x1="4.165" y1="-0.1" x2="4.165" y2="-0.975" width="0.0508" layer="21"/>
<pad name="2" x="-1" y="2.71" drill="0.95" shape="long" rot="R90"/>
<pad name="3" x="1" y="2.71" drill="0.95" shape="long" rot="R90"/>
<pad name="1" x="-3.5" y="2.71" drill="0.95" shape="long" rot="R90"/>
<pad name="4" x="3.5" y="2.71" drill="0.95" shape="long" rot="R90"/>
<text x="-7.62" y="5.08" size="1.27" layer="25">&gt;NAME</text>
<text x="9.144" y="-10.16" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<text x="-3.81" y="-2.04" size="0.4064" layer="21">E L E C T R O N I C S</text>
<hole x="-6.57" y="0" drill="2.3"/>
<hole x="6.57" y="0" drill="2.3"/>
</package>
</packages>
<symbols>
<symbol name="J11">
<wire x1="0" y1="7.62" x2="0" y2="-5.08" width="0.254" layer="94"/>
<wire x1="0" y1="-5.08" x2="5.08" y2="-5.08" width="0.254" layer="94"/>
<wire x1="0" y1="7.62" x2="5.08" y2="7.62" width="0.254" layer="94"/>
<text x="0" y="8.89" size="1.778" layer="95">&gt;NAME</text>
<text x="0" y="-7.62" size="1.778" layer="96">&gt;VALUE</text>
<text x="5.08" y="-2.54" size="2.54" layer="94" rot="R90">USB</text>
<pin name="1" x="-2.54" y="5.08" visible="pad" length="short" direction="pas"/>
<pin name="2" x="-2.54" y="2.54" visible="pad" length="short" direction="pas"/>
<pin name="3" x="-2.54" y="0" visible="pad" length="short" direction="pas"/>
<pin name="4" x="-2.54" y="-2.54" visible="pad" length="short" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="PN87520" prefix="X" uservalue="yes">
<description>&lt;b&gt;BERG&lt;/b&gt; USB connector</description>
<gates>
<gate name="G$1" symbol="J11" x="-2.54" y="0"/>
</gates>
<devices>
<device name="" package="PN87520">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="X2" library="con-tycoelectronics" deviceset="6ESRM-P" device=""/>
<part name="X1" library="con-berg" deviceset="PN87520" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<rectangle x1="35.56" y1="33.02" x2="86.36" y2="58.42" layer="91"/>
<text x="45.72" y="48.26" size="1.778" layer="94">SLUGS TECH CORPORATION</text>
<text x="48.26" y="43.18" size="1.778" layer="94">LIMITED CONFIDENTIAL</text>
</plain>
<instances>
<instance part="X2" gate="G$1" x="96.52" y="45.72"/>
<instance part="X1" gate="G$1" x="22.86" y="48.26" rot="R180"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="X1" gate="G$1" pin="4"/>
<pinref part="X2" gate="G$1" pin="SH"/>
<wire x1="25.4" y1="50.8" x2="30.48" y2="45.72" width="0.1524" layer="91"/>
<wire x1="30.48" y1="45.72" x2="93.98" y2="45.72" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="X1" gate="G$1" pin="1"/>
<pinref part="X2" gate="G$1" pin="AC2"/>
<wire x1="25.4" y1="43.18" x2="25.4" y2="40.64" width="0.1524" layer="91"/>
<wire x1="25.4" y1="40.64" x2="93.98" y2="40.64" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
