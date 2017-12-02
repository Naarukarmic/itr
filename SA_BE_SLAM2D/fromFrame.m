function [pw, PW_f, PW_pf] = fromFrame(F, pf)
%   FROMFRAME Transform a point PF from local frame F to the global frame.
%   
%   In: 
%       F :     reference frame     F  = [f_x ; f_y ; f_alpha]
%       pf:     point in frame F    pf = [pf_x ; pf_y]
%   Out:
%       pw:     point in global frame
%       PW_f:   Jacobian wrt F
%       PW_pf:  Jacobian wrt pf

%   (c) 2010, 2011, 2012 Joan Sola

t = F(1:2);
a = F(3);

R = [cos(a) -sin(a) ; sin(a) cos(a)];

pw = R*pf + repmat(t,1,size(pf,2));  % Allow for multiple points

if nargout > 1 %  Jacobians requested
    
    px = pf(1);
    py = pf(2);
    
    PW_f =  [...
        [ 1, 0, - py*cos(a) - px*sin(a)]
        [ 0, 1,   px*cos(a) - py*sin(a)]];
    
    PW_pf = R;
    
end
end

function f()
%% Symbolic code below -- Generation and/or test of Jacobians
% - Enable 'cell mode' to use this section
% - Left-click once on the code below - the cell should turn yellow
% - Type ctrl+enter (Windows, Linux) or Cmd+enter (MacOSX) to execute
% - Check the Jacobian results in the Command Window. 
syms x y a px py real
F = [x;y;a];
pf = [px;py];
pw = fromFrame(F,pf);
PW_f = jacobian(pw,F)
PW_pf = jacobian(pw,pf)
end
