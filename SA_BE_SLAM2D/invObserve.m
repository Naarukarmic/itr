function [p, P_r, P_y] = invObserve(r, y)
%   INVOBSERVE Backproject a range-and-bearing measurement and transform
%   to map frame.
%   
%   In: 
%       r :     robot frame     r = [r_x ; r_y ; r_alpha]
%       y :     measurement     y = [range ; bearing]
%   Out:
%       p :     point in global frame
%       P_r:    Jacobian wrt r
%       P_y:    Jacobian wrt y

%   (c) 2010, 2011, 2012 Joan Sola

if nargout == 1 % No Jacobians requested
    
    p   = fromFrame(r, invScan(y));

else %  Jacobians requested
    
    [p_r, PR_y]    = invScan(y);
    [p, P_r, P_pr] = fromFrame(r, p_r);
    
    % here the chain rule !
    P_y = P_pr * PR_y;
        
end
end

function f()
%% Symbolic code below -- Generation and/or test of Jacobians
% - Enable 'cell mode' to use this section
% - Left-click once on the code below - the cell should turn yellow
% - Type ctrl+enter (Windows, Linux) or Cmd+enter (MacOSX) to execute
% - Check the Jacobian results in the Command Window. 
syms rx ry ra yd ya real
r = [rx;ry;ra];
y = [yd;ya];
[p, P_r, P_y] = invObserve(r, y); % We extract also the coded Jacobians P_r and P_y
% We use the symbolic result to test the coded Jacobians
simplify(P_r - jacobian(p,r))  % zero-matrix if coded Jacobian is correct
simplify(P_y - jacobian(p,y))  % zero-matrix if coded Jacobian is correct
end