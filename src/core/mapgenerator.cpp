#include "mapgenerator.h"



bool operator<(WeightedLocation first, WeightedLocation second)
{
	return first.m_value< second.m_value;
}



void MapGenerator::createMapData(MapData* mdata, RegionData* rdata)
{
	mdata->m_base_map = new Matrix2d<char>(rdata->m_dimx,rdata->m_dimy);
	mdata->m_base_map->clear();
	mdata->m_environment = new Matrix2d<float>(rdata->m_dimx,rdata->m_dimy);
	mdata->m_region = new Region(rdata->m_dimx,rdata->m_dimy,rdata->m_id);

}

void MapGenerator::createBaseMap(MapData* mdata, RegionData* rdata)
{
	// Umgebungen in die Region einfuegen
	
	std::list<std::pair<float, EnvironmentName> >::iterator et;
	for (et = rdata->m_environments.begin(); et != rdata->m_environments.end(); ++et)
	{
		mdata->m_region->insertEnvironment(et->first,et->second);
	}
	
	float size = rdata->m_area_percent;
			
	int dimx = rdata->m_dimx;
	int dimy = rdata->m_dimy;
	
	// temporaere Karte anlegen
	Matrix2d<float>* hmap = new Matrix2d<float>(dimx, dimy);
	
	hmap->clear();
	
	// Karte erzeugen mit Perlin Noise
	// erhaltenes Feld wird als Hoehenkarte interpretiert
	// alle Felder die unterhalb eines kritischen Niveaus liegen sind begehbar
	createPerlinNoise(hmap,dimx, dimy, rdata->m_granularity, rdata->m_complexity,true);
	
	
	// Delta zu Nachbarfeldern
	int nb[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
	int dnb[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,1},{1,1},{1,-1},{-1,-1}};
	
	// Suchen eine Niveaus das eine hinreichend grosse zusammenhaengende Flaeche erzeugt
	// Idee: alle Felder werden in einer PriorityQueue eingeordnet
	// alle Felder werden in einen UnionFind Baum eingeordnet
	// von kleinster Höhe beginnend werden die Felder jeweils mit dem tiefstliegenden Nachbarfeld verschmolzen
	
	std::priority_queue<WeightedLocation> fields_queue;
	WeightedLocation loc;
	for (int i=0; i< dimx; i++)
	{
		for (int j=0; j< dimy; j++)
		{
			loc.m_x = i;
			loc.m_y = j;
			loc.m_value = 1- (*hmap)[i][j];
			
			fields_queue.push(loc);
		}
	}
	
	UnionFindTree uftree(dimx*dimy);
	
	float height;
	int x,y,nbx, nby;
	
	// Solange *Wasserstand* erhoehen, bis eine genuegend grosse zusammenhaengende Flaeche gefunden wurde
	while (1)
	{
		// oberstes Feld aus der Queue nehmen
		loc = fields_queue.top();
		fields_queue.pop();
		
		height = 1-loc.m_value;
		x = loc.m_x;
		y = loc.m_y;
		
		// Schleife ueber die Nachbarfelder
		for (int i=0; i<4; i++)
		{
			nbx = x + nb[i][0];
			nby = y + nb[i][1];
			
			// Dimension pruefen
			if (nbx<0 || nby<0 || nbx >= dimx || nby >= dimy)
				continue;
			
			if ( (*hmap)[nbx][nby] <= height)
			{
				uftree.merge(nbx*dimy+nby,x*dimy+y);
			}
		}
		
		// Testen ob eine hinreichend große Flaeche entstanden ist
		if (uftree.getCardinality(x*dimy+y) >= dimx*dimy*size)
		{
			break;
		}
		
	}
	
	// kleiner Aufschlag um sehr enge Durchgaenge zu vermeiden
	height += 0.01;
	
	// Floodfill um die Flaeche und deren Rand zu finden
	// die besuchten Felder werden in border mit 1 markiert
	// es werden nur Felder besucht, die unterhalb der gefundenen Hoehe liegen
	int cnt=0;
	std::queue<std::pair<int,int> > qu;
	std::pair<int,int> point;
	
	// Queue mit Felder die zum Rand gehoeren
	std::queue<std::pair<int,int> > borderqu;
	
	qu.push(std::make_pair(x,y));
	(*(mdata->m_base_map))[x][y]=1;
	
	// Solange die Queue nicht leer ist
	
	while (!qu.empty())
	{
		// Feld entnehmen
		point = qu.front();
		qu.pop();
		
		x = point.first;
		y = point.second;
		
		// Schleife ueber die Nachbarfelder
		for (int i=0; i<4; i++)
		{
			nbx = x + nb[i][0];
			nby = y + nb[i][1];
			
			// Dimension pruefen
			if (nbx<0 || nby<0 || nbx >= dimx || nby >= dimy)
				continue;
			
			// nur noch nicht besuchte Felder
			if ((*(mdata->m_base_map))[nbx][nby] != 0)
				continue;
			
			// Felder oberhalb der Obergrenze zum Rand hinzufuegen
			if ((*hmap)[nbx][nby] > height)
			{
				borderqu.push(std::make_pair(nbx,nby));
				(*(mdata->m_base_map))[nbx][nby]=-1;
				
				mdata->m_border.push_back(std::make_pair(nbx,nby));
				continue;
			}
			
			// Feld hinzufuegen
			qu.push(std::make_pair(nbx,nby));
			// als besucht markieren
			(*(mdata->m_base_map))[nbx][nby]=1;
			
			cnt++;
		}
	}
	//std::cout << "area percent: "<<cnt*1.0/(dimx*dimy)<<"\n";
	
	// nochmal 5 Runden Floodfill um den Rand zu ermitteln
	
	// Marker einfuegen
	int markercnt =0;
	borderqu.push(std::make_pair(-1,-1));
	
	
	while (!borderqu.empty() && markercnt <5)
	{
		// Feld entnehmen
		point = borderqu.front();
		borderqu.pop();
		
		x = point.first;
		y = point.second;
		
		// Testen ob der Marker entnommen wurde
		if (x==-1)
		{
			markercnt ++;
			borderqu.push(std::make_pair(-1,-1));
			continue;
		}
		
		// Schleife ueber die Nachbarfelder
		for (int i=0; i<8; i++)
		{
			nbx = x + dnb[i][0];
			nby = y + dnb[i][1];
			
			// Dimension pruefen
			if (nbx<0 || nby<0 || nbx >= dimx || nby >= dimy)
				continue;
			
			// nur noch nicht besuchte Felder
			if ((*(mdata->m_base_map))[nbx][nby] != 0)
				continue;
			
			
			// Feld hinzufuegen
			borderqu.push(std::make_pair(nbx,nby));
			// als besucht markieren
			(*(mdata->m_base_map))[nbx][nby]=-1;
		}
	}
	
	// Umgebungskarte generieren
	//createPerlinNoise(mdata->m_environment, dimx, dimy,4 , 0.4,false);
}

void MapGenerator::createBorder(MapData* mdata, RegionData* rdata)
{
	// Delta zu Nachbarfeldern
	int nb[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
	int dnb[8][2] = {{-1,0},{1,0},{0,-1},{0,1},{-1,1},{1,1},{1,-1},{-1,-1}};
	
	// Matrix die angibt, welche 8x8 Felder blockiert sind
	// Felder werden markiert, wenn alle 4x4 Unterfelder blockiert sind
	int hdimx = rdata->m_dimx/2;
	int hdimy = rdata->m_dimy/2;
	Matrix2d<char> bmap(hdimx,hdimy);
	bmap.clear();
	
	for (int i=0; i<hdimx; i++)
	{
		for (int j=0; j< hdimy;j++)
		{
			
			// Randfeld wird immer als blockiert markiert
			if (i==0 || j==0 || i==hdimx-1 || j== hdimy-1)
			{
				*(bmap.ind(i,j)) = 1;
				continue;
			}
			
			// Feld wird als blockiert markiert, wenn alle 4x4 Unterfelder blockiert sind
			if (*(mdata->m_base_map->ind(2*i,2*j)) == -1 &&  *(mdata->m_base_map->ind(2*i+1,2*j))== -1
						   && *(mdata->m_base_map->ind(2*i,2*j+1)) == -1 && *(mdata->m_base_map->ind(2*i+1,2*j+1)) == -1)
			{
				*(bmap.ind(i,j)) = 1;
			}
			else
			{
				if (*(mdata->m_base_map->ind(2*i,2*j)) >= 1 ||  *(mdata->m_base_map->ind(2*i+1,2*j))>= 1
								  || *(mdata->m_base_map->ind(2*i,2*j+1)) >=1  || *(mdata->m_base_map->ind(2*i+1,2*j+1)) >= 1)
				{
					*(mdata->m_base_map->ind(2*i,2*j)) =1;
					*(mdata->m_base_map->ind(2*i+1,2*j)) =1;
					*(mdata->m_base_map->ind(2*i,2*j+1)) =1;
					*(mdata->m_base_map->ind(2*i+1,2*j+1)) =1;
					
					*(bmap.ind(i,j)) = -1;
				}
				
			}
		}
	}
	
	// fuer jedes markierte werden die 4 Nachbarfelder angesehen
	// die 4 Informationen werden als 4bit Zahl interpretiert
	// es gibt 6 verschiedene Konstellationen fuer die Nachbarfelder
	// die 4bit Zahl dient als Index um die Konstellation und den Drehwinkel zu erhalten
	
	// Liste mit den 6 verschiedenen Moeglichenkeiten welche Nachbarfelder besetzt sind
	std::string borders[6] = {"border(side)","border(corner)","border(twoside)","border(twocorner)","border(filled)","border(single_block)"};
	
	// Maske dafuer, welche Situation vorliegt
	int bmask[16]={4,0,0,2,0,1,1,3,0,1,1,3,2,3,3,5};
	
	// Maske fuer die Rotationswinkel in Schritten von 90 Grad
	int rotmask[16] = {0,0,2,0,3,0,3,0,1,1,2,2,1,1,3,0};
	int mask =0;
	int nbi, nbj;
	
	ObjectGroupTemplateName templ;
	float angle;
	
	bool skip;
	
	Matrix2d<char> maskmap(hdimx,hdimy);
	maskmap.clear();
	
	for (int i=0; i<hdimx; i++)
	{
		for (int j=0; j< hdimy;j++)
		{
			skip = false;
			if (*(bmap.ind(i,j)) != 1)
			{
				continue;
			}
			
			mask =0;
			// Bitmaske aufbauen
			for (int k=0; k<4; k++)
			{
				nbi = i + nb[k][0];
				nbj = j + nb[k][1];
				
				mask *=2;
				if ( nbi<0 || nbj<0 || nbi>=hdimx || nbj>=hdimy)
				{
					continue;
				}
				
				if (*(bmap.ind(nbi,nbj)) == -1)
				{
					mask +=1;
				}
				
			}
			
			// Template und dessen Winkel bestimmen
			templ = borders[bmask[mask]];
			angle = rotmask[mask] *PI/2;
			
			DEBUG5("placing type (%i %i) %i %i %s",i,j,mask, bmask[mask]+1, borders[bmask[mask]].c_str());
			mdata->m_region->createObjectGroup(templ,Vector(i*8+4,j*8+4),angle);
			//std::cout << "\n";
			
			*(maskmap.ind(i,j)) = bmask[mask]+1;
			
		}
	}
	
	

}

void MapGenerator::createPerlinNoise(Matrix2d<float> *data, int dimx, int dimy,int startfreq, float persistance, bool bounds)
{
	Matrix2d<float>* tmp= new Matrix2d<float>(dimx+1, dimy+1);
	tmp->clear();
	float* weightx = new float[dimx];
	float* weighty = new float[dimy];
	int freqy,px,py,x,y;
	float ampl =1;
	float amp = ampl;
	
	// Perlin Noise Algorithmus
	for (int freq = startfreq; freq <= dimx;freq*=2)
	{
		freqy = (freq * dimy)/dimx;
		
		// anlegen der Zufallszahlen fuer die aktuelle Frequenz
		for (int i=0;i<=freq;i++)
		{
			for (int j=0;j<=freqy;j++)
			{
				*(tmp->ind(i,j)) = Random::random()* ampl;
				
			}
			
		}
		
		// Interpolation
		px = dimx/freq;
		float invpx = 1.0/ px;
		py = dimy/freqy;
		float invpy = 1.0/ py;
		
		for (int k=0;k<px;k++)
		{
			weightx[k] = k*invpx;
		}
		
		for (int l=0;l<py;l++)
		{
			weighty[l] = l*invpy;
		}
		
		for (int i=0;i<freq;i++)
		{
			for (int j=0;j<freqy;j++)
			{
				for (int k=0;k<px;k++)
				{
					for (int l=0;l<py;l++)
					{
						
						*(data->ind(i*px+k,j*py+l)) += (*(tmp->ind(i,j))*(1-weightx[k])*(1-weighty[l]) 
								+ *(tmp->ind(i+1,j))*weightx[k]*(1-weighty[l])
								+ *(tmp->ind(i+1,j+1))*weightx[k]*weighty[l]
								+ *(tmp->ind(i,j+1))*(1-weightx[k])*weighty[l]) ;
					}
				}
			}
		}	
		
		
		ampl *= persistance;
	}
	
	// Maximum und Minimum suchen
	float fmax =0,fmin = 1000000;
	for (int i=0;i<dimx;i++)
	{
		for (int j=0;j<dimy;j++)
		{
			if (*(data->ind(i,j)) > fmax)
			{
				fmax = *(data->ind(i,j));
			}
			if (*(data->ind(i,j)) < fmin)
			{
				fmin = *(data->ind(i,j));
			}

			
		}
	}
	
	// normieren auf [0..1]
	for (int i=0;i<dimx;i++)
	{
		for (int j=0;j<dimy;j++)
		{
			*(data->ind(i,j)) -= fmin;
			if (fmax-fmin !=0)
			{
				*(data->ind(i,j)) /= (fmax-fmin);
			}
		}
	}
	
	// Raender anlegen
	if (bounds)
	{
		float bnd = 0.1;
		float dist;
		for (int i=0;i<dimx;i++)
		{
			for (int j=0;j<dimy;j++)
			{
				dist = (bnd + std::max(fabs(i-dimx/2)/dimx,fabs(j-dimy/2)/dimy)-0.5);
				if (dist>0)
				{
					*(data->ind(i,j)) = *(data->ind(i,j))*(bnd-dist)/bnd + 1*dist/bnd;
				}
			}
		}
	}
	
	delete weightx;
	delete weighty;
	delete tmp;		
}



